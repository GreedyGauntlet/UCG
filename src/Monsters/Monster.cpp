#include "Monster.h"
#include "Flora/Math/Math.h"
#include "../Utils/SceneUtils.h"
#include "Flora/Renderer/Renderer2D.h"

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
		Destroy();
	}

	void Monster::DrawHealth() {
		bool hovered = false;
		if (m_Context->HoveredEntity())
			if (*m_Context->HoveredEntity() == m_Tile || *m_Context->HoveredEntity() == m_Body) hovered = true;
		if ((m_Status.Health != m_Status.MaxHealth || hovered) && m_Status.Health > 0) {
			glm::vec3 translation = m_Body.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.0f, -0.9f, 0.1f);
			Flora::Renderer2D::BeginScene(SceneUtils::MainCamera()->GetProjection());
			float node_width = 0.035f;
			float padding = 0.02f;
			float width = (node_width + padding) * m_Status.MaxHealth + padding;
			Flora::Renderer2D::DrawQuad(Flora::Math::ComposeTransform(translation, { 0.0f, 0.0f, 0.0f }, { width, 0.08, 1.0f }), { 0.0f, 0.0f, 0.0f, 1.0f });
			for (int i = 0; i < m_Status.MaxHealth; i++) {
				glm::vec4 color = i + 1 <= m_Status.Health ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
			switch (m_Status.Orientation) {
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
					m_Status.Orientation = std::get<1>(m_AnimationQueue.CurrentAnimation);
					Animation curr = GetAnimation(m_AnimationQueue.CurrentAnimation);
					Flora::SpriteRendererComponent& src = m_Body.GetComponent<Flora::SpriteRendererComponent>();
					src.StartFrame = std::get<0>(curr);
					src.Time = 0.0f;
					src.CurrentFrame = std::get<0>(curr);
					src.EndFrame = std::get<1>(curr);
					src.FPS = std::get<2>(curr);
					if (m_Status.Orientation == Orientation::DR || m_Status.Orientation == Orientation::UR)
						m_Body.GetComponent<Flora::TransformComponent>().Rotation = { 0, glm::radians(180.0f), 0 };
					else
						m_Body.GetComponent<Flora::TransformComponent>().Rotation = glm::vec3(0);
				}
			}
			m_AnimationQueue.AnimationTime += ts;
		}
	}

}