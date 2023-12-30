#include "Sanctuary.h"
#include "../Utils/FileUtils.h"

namespace UCG {

	void Sanctuary::Initialize(BattleScene* scene, Flora::Entity tile) {
		m_VFX = scene->CreateEntity("VFX Sanctuary");
		m_Tile = tile;
		m_Scene = scene;
		Flora::SpriteRendererComponent& src = m_VFX.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& tc = m_VFX.GetComponent<Flora::TransformComponent>();
		tc.Translation.z = 0.011f;
		tc.Scale = { 1.0f, 1.0f, 1.0f };
		src.Path = UCG::FileUtils::Path("assets/VFX/Sanctuary.png");
		if (!m_Tile.HasComponent<Flora::ChildComponent>()) m_Tile.AddComponent<Flora::ChildComponent>();
		m_VFX.AddComponent<Flora::ParentComponent>().Parent = m_VFX;
		m_Tile.GetComponent<Flora::ChildComponent>().AddChild(m_VFX);
	}

	bool Sanctuary::Update() {
        
		return true;
	}

	bool Sanctuary::Activate() {
        
        return true;
	}

}