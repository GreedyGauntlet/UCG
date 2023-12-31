#include "Sanctuary.h"
#include "../Utils/FileUtils.h"
#include "Flora/Core/Log.h"

namespace UCG {

	void Sanctuary::Initialize(BattleScene* scene, Flora::Entity tile) {
		m_VFX = scene->CreateEntity("VFX Sanctuary");
		m_Tile = tile;
		m_Scene = scene;
		m_TileCoords = m_Scene->GetTileCoords(m_Tile);
		Flora::SpriteRendererComponent& src = m_VFX.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& tc = m_VFX.GetComponent<Flora::TransformComponent>();
		tc.Translation.z = 0.011f;
		tc.Scale = { 1.0f, 1.0f, 1.0f };
		src.Path = UCG::FileUtils::Path("assets/VFX/Sanctuary.png");
		if (!m_Tile.HasComponent<Flora::ChildComponent>()) m_Tile.AddComponent<Flora::ChildComponent>();
		m_VFX.AddComponent<Flora::ParentComponent>().Parent = m_Tile;
		m_Tile.GetComponent<Flora::ChildComponent>().AddChild(m_VFX);
	}

	bool Sanctuary::TurnUpdate() {
    m_Health--;
		return m_Health > 0;
	}

	void Sanctuary::PersistantUpdate() {
	  Monster* occupied = m_Scene->GetMonster(m_TileCoords.first, m_TileCoords.second);
    if (occupied) {
      occupied->Body().GetComponent<Flora::SpriteRendererComponent>().Color = {1.0, 0.5f, 0.5f, 1.0f}; 
    }
  }
}
