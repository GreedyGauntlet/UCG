#include "Building.h"

namespace UCG {
	void Building::Initialize(GameScene* context, Flora::Entity tile) {
		m_Context = context;
		m_Tile = GetTileRef(tile);
		m_Body = m_Context->CreateEntity("Building");
		m_Body.AddComponent<Flora::ParentComponent>().Parent = tile;

		Flora::SpriteRendererComponent& src = m_Body.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& tc = m_Body.GetComponent<Flora::TransformComponent>();
		tc.Translation.z = 0.01f;

		src.Path = tile.GetComponent<Flora::SpriteRendererComponent>().Path;
	}

	void Building::SetType(BuildingType type) {
		m_Type = type;
	}
}