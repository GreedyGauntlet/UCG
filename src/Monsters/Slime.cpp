#include "Slime.h"
#include "../Utils/FileUtils.h"
#include "../Scenes/BattleScene.h"

namespace UCG {

	void Slime::Initialize(BattleScene* context, TileRef tile) {
		m_Status.MaxHealth = m_Status.Health = 3;
		m_Context = context;
		m_Body = m_Context->CreateEntity("Slime");
		m_Tile = tile;
		Flora::TransformComponent& tc = m_Body.GetComponent<Flora::TransformComponent>();
		Flora::SpriteRendererComponent& src = m_Body.AddComponent<Flora::SpriteRendererComponent>();
		tc.Translation = context->GetTile(tile).Translation();
		tc.Translation.y += 0.75f;
		tc.Translation.z = 3.0f;
		tc.Scale = { 1.0f, 2.0f, 1.0f };
		src.Type = Flora::SpriteRendererComponent::SpriteType::ANIMATION;
		src.Path = UCG::FileUtils::Path("assets/Monsters/Slime.png");
		src.Frames = 30;
		src.StartFrame = 1;
		src.EndFrame = 7;
		src.FPS = 6;
		src.Rows = 1;
		src.Columns = 30;
	}

	void Slime::Update(Flora::Timestep ts) {
		Flora::SpriteRendererComponent& src = m_Body.GetComponent<Flora::SpriteRendererComponent>();
		if (src.CurrentFrame >= 6 && src.StartFrame == 1) {
			src.StartFrame = 8;
			src.EndFrame = 18;
		}
		DrawHealth();
		DamageAnim(ts);
	}

	void Slime::DeathAnim(Flora::Timestep ts) {
		Flora::SpriteRendererComponent& src = m_Body.GetComponent<Flora::SpriteRendererComponent>(); 
		src.StartFrame = 19;
		src.EndFrame = 30;
		src.FPS = 4;
		if (src.CurrentFrame >= 30) {
			Destroy();
		}
	}

}
