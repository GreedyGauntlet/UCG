#include "Goblin.h"

namespace UCG {

	void Goblin::Initialize(GameScene* context, Flora::Entity tile) {
		m_Status.MaxHealth = m_Status.Health = 5;
		m_Context = context;
		m_Body = m_Context->CreateEntity("Goblin");
		m_Tile = GetTileRef(tile);
		Flora::TransformComponent& tc = m_Body.GetComponent<Flora::TransformComponent>();
		Flora::SpriteRendererComponent& src = m_Body.AddComponent<Flora::SpriteRendererComponent>();
		tc.Translation = tile.GetComponent<Flora::TransformComponent>().Translation;
		tc.Translation.y += 0.75f;
		tc.Translation.z = 3.0f;
		tc.Scale = { 1.0f, 2.0f, 1.0f };
		src.Type = Flora::SpriteRendererComponent::SpriteType::ANIMATION;
		src.Path = "assets/Monsters/Goblin.png";
		src.Frames = 42;
		src.Rows = 1;
		src.Columns = 42;
		
		m_Animations.IdleDown = { 22, 28, 10 };
		m_Animations.IdleUp = { 29, 35, 10 };
		m_Animations.Spawn = { 8, 14, 10 };
		m_Animations.DeathDown = { 1, 7, 10 };

		QueueAnimation({AnimationState::SPAWN, Orientation::NONE});
		QueueAnimation({AnimationState::IDLE, Orientation::DL});
	}

	void Goblin::StartTurn() {
		PushAction(Action::MOVE);
		PushAction(Action::ATTACK);
		PushAction(Action::IDLE);
	}

}