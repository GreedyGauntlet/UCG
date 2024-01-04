#include "Meteor.h"
#include "../Utils/FileUtils.h"

namespace UCG {

	void Meteor::Initialize(BattleScene* scene, TileRef tile) {
		m_VFX = scene->CreateEntity("VFX Meteor");
		m_Tile = tile;
		m_Scene = scene;
		Flora::SpriteRendererComponent& src = m_VFX.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& tc = m_VFX.GetComponent<Flora::TransformComponent>();
		tc.Translation = scene->GetTile(tile).Contents.Body.GetComponent<Flora::TransformComponent>().Translation;
		tc.Translation.y += 0.75f;
		tc.Translation.z = 3.0f;
		tc.Scale = { 1.0f, 2.0f, 1.0f };
		src.Path = UCG::FileUtils::Path("assets/VFX/Meteor.png");
		src.Type = Flora::SpriteRendererComponent::SpriteType::ANIMATION;
		src.Rows = 1;
		src.Columns = 28;
		src.FPS = 20;
		src.Frames = 28;
		src.StartFrame = 1;
		src.EndFrame = 28;
	}

	bool Meteor::Update() {
		if (m_VFX.GetComponent<Flora::SpriteRendererComponent>().CurrentFrame == 28) {
			m_Scene->DestroyEntity(m_VFX);
			return false;
		}
		return true;
	}

	bool Meteor::Activate() {
		if (m_VFX.GetComponent<Flora::SpriteRendererComponent>().CurrentFrame >= 22 && !m_Active) {
			m_Active = true;
			return true;
		}
		return false;
	}

}
