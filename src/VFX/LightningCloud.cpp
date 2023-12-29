#include "LightningCloud.h"
#include "../Utils/FileUtils.h"

namespace UCG {

	void LightningCloud::Initialize(BattleScene* scene, Flora::Entity tile) {
		m_VFX = scene->CreateEntity("VFX Lightning");
		m_Tile = tile;
		m_Scene = scene;
		Flora::SpriteRendererComponent& src = m_VFX.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& tc = m_VFX.GetComponent<Flora::TransformComponent>();
		tc.Translation = tile.GetComponent<Flora::TransformComponent>().Translation;
		tc.Translation.y += 0.75f;
		tc.Translation.z = 3.0f;
		tc.Scale = { 1.0f, 2.0f, 1.0f };
		src.Path = UCG::FileUtils::Path("assets/VFX/Smite.png");
		src.Type = Flora::SpriteRendererComponent::SpriteType::ANIMATION;
		src.Rows = 1;
		src.Columns = 12;
		src.FPS = 10;
		src.Frames = 12;
		src.StartFrame = 1;
		src.EndFrame = 12;
	}

	bool LightningCloud::Update() {
		if (m_VFX.GetComponent<Flora::SpriteRendererComponent>().CurrentFrame == 12) {
			m_Scene->DestroyEntity(m_VFX);
			return false;
		}
		return true;
	}

	bool LightningCloud::Activate() {
		if (m_VFX.GetComponent<Flora::SpriteRendererComponent>().CurrentFrame >= 7 && !m_Active) {
			m_Active = true;
			return true;
		}
		return false;
	}

}