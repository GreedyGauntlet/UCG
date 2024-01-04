#include "Goblin.h"
#include "../Scenes/BattleScene.h"
#include "../Utils/RandomUtils.h"
#include "../Utils/FileUtils.h"
#include "Behaviors.h"

namespace UCG {

	void Goblin::Initialize(BattleScene* context, TileRef tile) {
		m_Status.MaxHealth = m_Status.Health = 5;
		m_Context = context;
		m_Body = m_Context->CreateEntity("Goblin");
		m_Tile = tile;
		Flora::TransformComponent& tc = m_Body.GetComponent<Flora::TransformComponent>();
		Flora::SpriteRendererComponent& src = m_Body.AddComponent<Flora::SpriteRendererComponent>();
		tc.Translation = m_Context->GetTile(tile).Contents.Body.GetComponent<Flora::TransformComponent>().Translation;
		tc.Translation.y += 0.75f;
		tc.Translation.z = 3.0f;
		tc.Scale = { 1.0f, 2.0f, 1.0f };
		src.Type = Flora::SpriteRendererComponent::SpriteType::ANIMATION;
		src.Path = UCG::FileUtils::Path("assets/Monsters/Goblin.png");
		src.Frames = 42;
		src.Rows = 1;
		src.Columns = 42;
		
		m_Animations.IdleDown = { 22, 28, 10 };
		m_Animations.IdleUp = { 29, 35, 10 };
		m_Animations.Spawn = { 8, 14, 10 };
		m_Animations.DeathDown = { 1, 7, 10 };
		m_Animations.AttackDown = { 15, 21, 10 };
		m_Animations.AttackUp = { 36, 42, 10 };

		QueueAnimation({AnimationState::SPAWN, Orientation::NONE});
		QueueAnimation({AnimationState::IDLE, Orientation::DL});
	}

	void Goblin::Attack() {
		m_Context->GetMonster(FrontTile())->Damage(1);
	}

	void Goblin::StartTurn() {
		TileRef nexttile = FrontTile();
		TileSet los;
		los.push_back(nexttile);
		TileSet targets = Behaviors::Target(m_Context, *this, (ObjectSelectFlags)(ObjectSelectFlags::LINEAR | ObjectSelectFlags::DIRECTIONAL | ObjectSelectFlags::NEAR), 1, los);
		if (targets.size() > 0) {
			PushAction(Action::ATTACK);
			PushAction(Action::IDLE);
			return;
		}

		Orientation curr_orient = m_Status.Direction;
		bool rotate_right = true;
		int choice = RandomUtils::Random(0, 3);
		switch (choice) {
		case 0:
			rotate_right = true;
			break;
		case 1:
			rotate_right = false;
			break;
		case 2:
			rotate_right = true;
			PushAction(Action::ROTATE_R);
			curr_orient = RotateOrientation(curr_orient, rotate_right);
			break;
		case 3:
			rotate_right = false;
			PushAction(Action::ROTATE_L);
			curr_orient = RotateOrientation(curr_orient, rotate_right);
			break;
		}

		nexttile = FrontTile(curr_orient);	
		los.clear();
		los.push_back(nexttile);
		targets = Behaviors::Target(m_Context, *this, (ObjectSelectFlags)(ObjectSelectFlags::LINEAR | ObjectSelectFlags::DIRECTIONAL | ObjectSelectFlags::NEAR), 1, los);
		if (targets.size() > 0) {
			PushAction(Action::ATTACK);
			PushAction(Action::IDLE);
			return;
		}
		if (m_Context->ValidBoardCoord(nexttile) &&
			Tile(nexttile).GetComponent<Flora::TagComponent>().Tag == "D" &&
			!(m_Context->TileOccupied(nexttile))) {
			PushAction(Action::MOVE);
			PushAction(Action::IDLE);
			return;
		}

		if (rotate_right)
			PushAction(Action::ROTATE_R);
		else
			PushAction(Action::ROTATE_L);
		Orientation nextorient = RotateOrientation(curr_orient, rotate_right);
		while (nextorient != m_Status.Direction) {

			nexttile = FrontTile(nextorient);
			los.clear();
			los.push_back(nexttile);
			targets = Behaviors::Target(m_Context, *this, (ObjectSelectFlags)(ObjectSelectFlags::LINEAR | ObjectSelectFlags::DIRECTIONAL | ObjectSelectFlags::NEAR), 1, los);
			if (targets.size() > 0) {
				PushAction(Action::ATTACK);
				PushAction(Action::IDLE);
				return;
			}
			if (m_Context->ValidBoardCoord(nexttile) &&
				Tile(nexttile).GetComponent<Flora::TagComponent>().Tag == "D" &&
				!(m_Context)->TileOccupied(nexttile)) {
				PushAction(Action::MOVE);
				PushAction(Action::IDLE);
				return;
			}

			if (rotate_right)
				PushAction(Action::ROTATE_R);
			else
				PushAction(Action::ROTATE_L);
			nextorient = RotateOrientation(nextorient, rotate_right);
		}
	}

}
