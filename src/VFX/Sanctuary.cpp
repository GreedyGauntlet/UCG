#include "Sanctuary.h"
#include "../Utils/FileUtils.h"
#include "../Monsters/Monster.h"

namespace UCG {

	void Sanctuary::Initialize(BattleScene* scene, TileRef tile) {
		m_VFX = scene->CreateEntity("VFX Sanctuary");
		m_Tile = tile;
		m_Scene = scene;
		Flora::SpriteRendererComponent& src = m_VFX.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& tc = m_VFX.GetComponent<Flora::TransformComponent>();
		tc.Translation.z = 0.011f;
		tc.Scale = { 1.0f, 1.0f, 1.0f };
		src.Path = UCG::FileUtils::Path("assets/VFX/Sanctuary.png");
		Flora::Entity tile_ent = scene->GetTileObj(tile).Body;
		if (!tile_ent.HasComponent<Flora::ChildComponent>()) tile_ent.AddComponent<Flora::ChildComponent>();
		m_VFX.AddComponent<Flora::ParentComponent>().Parent = tile_ent;
		tile_ent.GetComponent<Flora::ChildComponent>().AddChild(m_VFX);
	}

	bool Sanctuary::TurnUpdate() {
		m_Health--;
		return m_Health > 0;
	}

	void Sanctuary::PersistantUpdate() {
		Monster* occupied = m_Scene->GetMonster(m_Tile);
		if (occupied) {
			occupied->SetAllegiance(m_PlayerAligned ? Allegiances::PLAYER : Allegiances::OPPONENT);
		}
	}
}
