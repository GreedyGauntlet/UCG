#include "Monster.h"
#include "Flora/Math/Math.h"
#include "../Utils/SceneUtils.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Utils/ComponentUtils.h"
#include "../Scenes/BattleScene.h"

namespace UCG {

	void Monster::Destroy() {
		m_Context->DestroyEntity(m_Body);
		m_Status.Dead = true;
	}

	void Monster::Damage(int damage) {
		m_Status.Health -= damage;
		m_Status.Damaged = true;
		m_Status.RelativeTime = 0.0f;
	}

	void Monster::Update(Flora::Timestep ts) {
		DrawHealth();
		DamageAnim(ts);
		UpdateAnimation(ts);
	}

	void Monster::DamageAnim(Flora::Timestep ts) {
		if (m_Status.Damaged) {
			m_Status.RelativeTime += ts;
			if (m_Status.RelativeTime < 0.1f || (m_Status.RelativeTime > 0.15f && m_Status.RelativeTime < 0.25f)) {
				m_Body.GetComponent<Flora::SpriteRendererComponent>().Color = glm::vec4(10.0f);
			} else m_Body.GetComponent<Flora::SpriteRendererComponent>().Color = glm::vec4(1.0f);
			if (m_Status.RelativeTime > 1.0f) {
				m_Status.RelativeTime = 0.0f;
				m_Status.Damaged = false;
			}
		}
		if (m_Status.Health <= 0) {
			DeathAnim(ts);
		}
	}

	void Monster::DeathAnim(Flora::Timestep ts) {
		if (std::get<0>(m_AnimationQueue.CurrentAnimation) != AnimationState::DEATH)
			OverrideAnimation({ AnimationState::DEATH, m_Status.Direction });

		if (m_AnimationQueue.AnimationTime > GetAnimationTime(GetAnimation({ AnimationState::DEATH, m_Status.Direction })))
			Destroy();
	}

	void Monster::DrawHealth() {
		bool hovered  = false;
		if (m_Context->HoveredEntity() >= 0)
			if ((uint32_t)(m_Context->HoveredEntity()) == (uint32_t)m_Body) hovered = true;
		if ((m_Status.Health != m_Status.MaxHealth || hovered) && m_Status.Health > 0) {
			glm::vec3 translation, rotation, scale;
			Flora::Math::DecomposeTransform(Flora::ComponentUtils::GetWorldTransform(m_Body), translation, rotation, scale);
			translation += glm::vec3(0.0f, -0.9f, 0.1f);
			Flora::Renderer2D::BeginScene(SceneUtils::MainCamera()->GetProjection());
			float node_width = 0.035f;
			float padding = 0.02f;
			float width = (node_width + padding) * m_Status.MaxHealth + padding;
			Flora::Renderer2D::DrawQuad(Flora::Math::ComposeTransform(translation, { 0.0f, 0.0f, 0.0f }, { width, 0.08, 1.0f }), { 0.0f, 0.0f, 0.0f, 1.0f });
			for (int i = 0; i < m_Status.MaxHealth; i++) {
        glm::vec4 fillcolor = m_Status.Allegiance == Allegiances::NONE ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) : (m_Status.Allegiance == Allegiances::PLAYER ? glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) : glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); 
				glm::vec4 color = i + 1 <= m_Status.Health ? fillcolor : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
				Flora::Renderer2D::DrawQuad(
					Flora::Math::ComposeTransform(
						translation + glm::vec3((padding * (i + 2)) + (node_width * i) - (width / 2.0f), 0.0f, 0.1f),
						{ 0.0f, 0.0f, 0.0f },
						{ node_width, 0.04f, 1.0f }),
					color
				);
			}
			Flora::Renderer2D::EndScene();
		}
	}

	void Monster::QueueAnimation(AnimationCommand command) {
		m_AnimationQueue.Queue.push_back(command);
	}

	void Monster::OverrideAnimation(AnimationCommand command) {
		m_AnimationQueue.CurrentAnimation = command;
		m_AnimationQueue.AnimationTime = 0.0f;
	}

	Animation Monster::GetAnimation(AnimationCommand command) {
		Orientation orientation = std::get<1>(command);
		switch (std::get<0>(command)) {
		case AnimationState::INVALID: return m_Animations.Spawn;
		case AnimationState::SPAWN: return m_Animations.Spawn;
		case AnimationState::DEATH:
			switch (orientation) {
			case Orientation::DL: return m_Animations.DeathDown;
			case Orientation::DR: return m_Animations.DeathDown;
			default: return m_Animations.DeathUp;
			}
		case AnimationState::IDLE:
			switch (orientation) {
			case Orientation::DL: return m_Animations.IdleDown;
			case Orientation::DR: return m_Animations.IdleDown;
			default: return m_Animations.IdleUp;
			}
		case AnimationState::ATTACK:
			switch (orientation) {
			case Orientation::DL: return m_Animations.AttackDown;
			case Orientation::DR: return m_Animations.AttackDown;
			default: return m_Animations.AttackUp;
			}
		case AnimationState::MOVE:
			switch (orientation) {
			case Orientation::DL: return m_Animations.MoveDown;
			case Orientation::DR: return m_Animations.MoveDown;
			default: return m_Animations.MoveUp;
			}
		case AnimationState::ROTATE:
			switch (m_Status.Direction) {
			case Orientation::DR:
				switch (orientation) {
				case Orientation::DR: return m_Animations.IdleDown;
				case Orientation::UL: return m_Animations.RotateTop;
				case Orientation::UR: return m_Animations.RotateUp;
				default: return m_Animations.RotateBottom;
				}
			case Orientation::UL:
				switch (orientation) {
				case Orientation::DR: return m_Animations.RotateDown;
				case Orientation::UL: return m_Animations.IdleUp;
				case Orientation::UR: return m_Animations.RotateTop;
				default: return m_Animations.RotateDown;
				}
			case Orientation::UR:
				switch (orientation) {
				case Orientation::DR: return m_Animations.RotateDown;
				case Orientation::UL: return m_Animations.RotateTop;
				case Orientation::UR: return m_Animations.IdleUp;
				default: return m_Animations.RotateBottom;
				}
			default:
				switch (orientation) {
				case Orientation::DR: return m_Animations.RotateBottom;
				case Orientation::UL: return m_Animations.RotateUp;
				case Orientation::UR: return m_Animations.RotateTop;
				default: return m_Animations.IdleDown;
				}
			}
		default:
			return m_Animations.Spawn;
		}
	}

	float Monster::GetAnimationTime(Animation anim) {
		if (((float)std::get<2>(anim)) <= 0) return 0.0f;
		int numframes = std::get<1>(anim) - std::get<0>(anim);
		return ((float)numframes) / ((float)std::get<2>(anim));
	}

	void Monster::UpdateAnimation(Flora::Timestep ts) {
		if (!m_Status.Dead) {
			if (std::get<0>(m_AnimationQueue.CurrentAnimation) == AnimationState::INVALID && m_AnimationQueue.Queue.size() > 0) {
				m_AnimationQueue.CurrentAnimation = m_AnimationQueue.Queue[0];
				m_AnimationQueue.Queue.erase(m_AnimationQueue.Queue.begin());
				m_AnimationQueue.AnimationTime = 0.0f;
			}
			if (std::get<0>(m_AnimationQueue.CurrentAnimation) != AnimationState::INVALID) {
				if (m_AnimationQueue.AnimationTime > GetAnimationTime(GetAnimation(m_AnimationQueue.CurrentAnimation))
					&& m_AnimationQueue.Queue.size() > 0) {
					m_AnimationQueue.CurrentAnimation = m_AnimationQueue.Queue[0];
					m_AnimationQueue.Queue.erase(m_AnimationQueue.Queue.begin());
					m_AnimationQueue.AnimationTime = 0.0f;
				}
				if (m_AnimationQueue.AnimationTime == 0.0f) {
					Animation curr = GetAnimation(m_AnimationQueue.CurrentAnimation);
					Flora::SpriteRendererComponent& src = m_Body.GetComponent<Flora::SpriteRendererComponent>();
					src.StartFrame = std::get<0>(curr);
					src.Time = 0.0f;
					src.CurrentFrame = std::get<0>(curr);
					src.EndFrame = std::get<1>(curr);
					src.FPS = std::get<2>(curr);
					if (std::get<1>(m_AnimationQueue.CurrentAnimation) == Orientation::DR || std::get<1>(m_AnimationQueue.CurrentAnimation) == Orientation::UR)
						m_Body.GetComponent<Flora::TransformComponent>().Rotation = { 0, glm::radians(180.0f), 0 };
					else
						m_Body.GetComponent<Flora::TransformComponent>().Rotation = glm::vec3(0);
				}
			}
			m_AnimationQueue.AnimationTime += ts;
		}
	}

	void Monster::StartTurn() {
		PushAction(Action::IDLE);
	}

	void Monster::Attack() {
		// TODO: default behavior to just get the thing in front and attack it
		FL_CORE_WARN("Implement default attack please");
	}

	bool Monster::AttackAction(Flora::Timestep ts) {
		static bool attacked = false;
		if (m_ActionQueue.ActionTime == 0.0f) {
			attacked = false;
			if (ValidAnimation(GetAnimation({ AnimationState::ATTACK, m_Status.Direction }))) {
				OverrideAnimation({ AnimationState::ATTACK, m_Status.Direction });
				m_ActionQueue.TimeThreshold = GetAnimationTime(GetAnimation({ AnimationState::ATTACK, m_Status.Direction }));
			}
			else {
				OverrideAnimation({ AnimationState::IDLE, m_Status.Direction });
				m_ActionQueue.TimeThreshold = 0.5f;
			}
		}
		m_ActionQueue.ActionTime += ts;

		if (!attacked) {
			float fps = ((float)std::get<2>((GetAnimation({ AnimationState::ATTACK, m_Status.Direction }))));
			float max_attack_time = fps <= 0 ? 0.0f : ((float)m_ActionQueue.AttackFrame) / fps;
			if (m_ActionQueue.ActionTime >= max_attack_time) {
				attacked = true;
				Attack();
			}
		}

		if (m_ActionQueue.ActionTime > m_ActionQueue.TimeThreshold) {
			attacked = false;
			return false;
		}
		return true;
	}

	bool Monster::MoveAction(Flora::Timestep ts) {
		if (m_ActionQueue.ActionTime == 0.0f) {
			if (ValidAnimation(GetAnimation({ AnimationState::MOVE, m_Status.Direction }))) {
				OverrideAnimation({ AnimationState::MOVE, m_Status.Direction });
				m_ActionQueue.TimeThreshold = GetAnimationTime(GetAnimation({ AnimationState::MOVE, m_Status.Direction }));
			} else {
				OverrideAnimation({ AnimationState::IDLE, m_Status.Direction });
				m_ActionQueue.TimeThreshold = 0.3f;
			}
		}
		m_ActionQueue.ActionTime += ts;

		std::vector<std::vector<TileObj>> tiles = ((BattleScene*)m_Context)->GetBoardTiles();
		TileRef nextTile = m_Tile;
		switch (m_Status.Direction) {
		case Orientation::DR:
			if (m_Tile.second >= (int)(tiles[0].size() - 1)) return false;
			nextTile.second += 1;
			break;
		case Orientation::UL:
			if (m_Tile.second <= 0) return false;
			nextTile.second -= 1;
			break;
		case Orientation::UR:
			if (m_Tile.first <= 0) return false;
			nextTile.first -= 1;
			break;
		default:
			if (m_Tile.first >= (int)(tiles.size() - 1)) return false;
			nextTile.first += 1;
			break;
		}

		float stable_ts = m_ActionQueue.ActionTime > m_ActionQueue.TimeThreshold ? (ts - (m_ActionQueue.ActionTime - m_ActionQueue.TimeThreshold)) : (float)ts;

		glm::vec3 move_vec =
			((((BattleScene*)m_Context)->GetBoardTiles()[nextTile.first][nextTile.second].second.GetComponent<Flora::TransformComponent>().Translation
			-
			((BattleScene*)m_Context)->GetBoardTiles()[m_Tile.first][m_Tile.second].second.GetComponent<Flora::TransformComponent>().Translation)
			/ 
			m_ActionQueue.TimeThreshold)
			*
			stable_ts;

		m_Body.GetComponent<Flora::TransformComponent>().Translation += move_vec;

		if (m_ActionQueue.ActionTime > m_ActionQueue.TimeThreshold) {
			m_Tile = nextTile;
			return false;
		}
		return true;
	}

	Orientation Monster::RotateOrientation(Orientation origin, bool rotateright) {
		switch (origin) {
		case Orientation::UL: return rotateright ? Orientation::UR : Orientation::DL;
		case Orientation::DR: return rotateright ? Orientation::DL : Orientation::UR;
		case Orientation::UR: return rotateright ? Orientation::DR : Orientation::UL;
		default: return rotateright ? Orientation::UL : Orientation::DR;
		}
	}

	bool Monster::RotateRightAction(Flora::Timestep ts) {
		Orientation next_orientation = RotateOrientation(m_Status.Direction, true);
		if (m_ActionQueue.ActionTime == 0.0f) {
			if (ValidAnimation(GetAnimation({ AnimationState::ROTATE, next_orientation }))) {
				OverrideAnimation({ AnimationState::ROTATE, next_orientation });
				m_ActionQueue.TimeThreshold = GetAnimationTime(GetAnimation({ AnimationState::ROTATE, next_orientation }));
			}
			else {
				OverrideAnimation({ AnimationState::IDLE, next_orientation });
				m_ActionQueue.TimeThreshold = 0.2f;
			}
		}
		m_ActionQueue.ActionTime += ts;
		if (m_ActionQueue.ActionTime > m_ActionQueue.TimeThreshold) {
			m_Status.Direction = next_orientation;
			return false;
		}
		return true;
	}

	bool Monster::RotateLeftAction(Flora::Timestep ts) {
		Orientation next_orientation = RotateOrientation(m_Status.Direction, false);
		if (m_ActionQueue.ActionTime == 0.0f) {
			if (ValidAnimation(GetAnimation({ AnimationState::ROTATE, next_orientation }))) {
				OverrideAnimation({ AnimationState::ROTATE, next_orientation });
				m_ActionQueue.TimeThreshold = GetAnimationTime(GetAnimation({ AnimationState::ROTATE, next_orientation }));
			}
			else {
				OverrideAnimation({ AnimationState::IDLE, next_orientation });
				m_ActionQueue.TimeThreshold = 0.2f;
			}
		}
		m_ActionQueue.ActionTime += ts;
		if (m_ActionQueue.ActionTime > m_ActionQueue.TimeThreshold) {
			m_Status.Direction = next_orientation;
			return false;
		}
		return true;
	}

	void Monster::PushAction(Action action) {
		m_ActionQueue.Queue.push_back(action);
	}

	bool Monster::ValidAnimation(Animation anim) {
		return !(std::get<0>(anim) == 0 && std::get<1>(anim) == 0 && std::get<2>(anim) == 0);
	}

	TileRef Monster::FrontTile() {
		return FrontTile(m_Status.Direction);
	}

	TileRef Monster::FrontTile(Orientation orientation) {
		switch (orientation) {
		case Orientation::DR: return { m_Tile.first, m_Tile.second + 1 };
		case Orientation::UR: return { m_Tile.first - 1, m_Tile.second };
		case Orientation::UL: return { m_Tile.first, m_Tile.second - 1 };
		default: break;
		}
		return { m_Tile.first + 1, m_Tile.second };
	}

	void Monster::UpdateActions(Flora::Timestep ts) {
		if (m_Primed) {
			StartTurn();
			m_Primed = false;
		}

		if (m_ActionQueue.CurrentAction == Action::IDLE && m_ActionQueue.Queue.size() > 0) {
			m_ActionQueue.ActionTime = 0.0f;
			m_ActionQueue.CurrentAction = m_ActionQueue.Queue[0];
			m_ActionQueue.Queue.erase(m_ActionQueue.Queue.begin());
		}
		if (m_ActionQueue.CurrentAction != Action::IDLE) {
			switch (m_ActionQueue.CurrentAction) {
			case Action::ATTACK:
				if (!AttackAction(ts)) m_ActionQueue.CurrentAction = Action::IDLE;
				break;
			case Action::MOVE:
				if (!MoveAction(ts)) m_ActionQueue.CurrentAction = Action::IDLE;
				break;
			case Action::ROTATE_R:
				if (!RotateRightAction(ts)) m_ActionQueue.CurrentAction = Action::IDLE;
				break;
			case Action::ROTATE_L:
				if (!RotateLeftAction(ts)) m_ActionQueue.CurrentAction = Action::IDLE;
				break;
			default:
				break;
			}
		}
		if (m_ActionQueue.CurrentAction == Action::IDLE)
			OverrideAnimation({ AnimationState::IDLE, m_Status.Direction });
	}

	Flora::Entity Monster::Tile() {
		return Tile(m_Tile);
	}

	Flora::Entity Monster::Tile(TileRef ref) {
		return ((BattleScene*)m_Context)->GetBoardTiles()[ref.first][ref.second].second;
	}

	TileRef Monster::GetTileRef(Flora::Entity tile) {
		std::vector<std::vector<TileObj>> tiles = ((BattleScene*)m_Context)->GetBoardTiles();
		for (size_t r = 0; r < tiles.size(); r++) {
			for (size_t c = 0; c < tiles[0].size(); c++) {
				if (tiles[r][c].second == tile) {
					return { r, c };
				}
			}
		}
		return { 0, 0 };
	}

}
