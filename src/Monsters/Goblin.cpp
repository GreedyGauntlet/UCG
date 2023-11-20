#include "Goblin.h"

namespace UCG {

	void Goblin::Initialize(GameScene* context, Flora::Entity tile) {
		m_Status.MaxHealth = m_Status.Health = 5;
		m_Context = context;
		m_Body = m_Context->CreateEntity("Goblin");
		m_Tile = tile;
		Flora::TransformComponent& tc = m_Body.GetComponent<Flora::TransformComponent>();
		Flora::SpriteRendererComponent& src = m_Body.AddComponent<Flora::SpriteRendererComponent>();
		tc.Translation = tile.GetComponent<Flora::TransformComponent>().Translation;
		tc.Translation.y += 0.75f;
		tc.Translation.z = 3.0f;
		tc.Scale = { 1.0f, 2.0f, 1.0f };
		src.Type = Flora::SpriteRendererComponent::SpriteType::ANIMATION;
		src.Path = "assets/Monsters/Goblin.png";
		src.Frames = 42;
		src.StartFrame = 8;
		src.EndFrame = 14;
		src.FPS = 5;
		src.Rows = 1;
		src.Columns = 42;
	}

	void Goblin::Update(Flora::Timestep ts) {
		Flora::SpriteRendererComponent& src = m_Body.GetComponent<Flora::SpriteRendererComponent>();
		if (src.CurrentFrame >= 14 && src.StartFrame == 8) {
			src.StartFrame = 22;
			src.EndFrame = 28;
		}
		DrawHealth();
		DamageAnim(ts);
	}

	void Goblin::DeathAnim(Flora::Timestep ts) {
		Flora::SpriteRendererComponent& src = m_Body.GetComponent<Flora::SpriteRendererComponent>();
		src.StartFrame = 1;
		src.EndFrame = 7;
		src.FPS = 5;
		if (src.CurrentFrame >= 7) {
			Destroy();
		}
	}

}