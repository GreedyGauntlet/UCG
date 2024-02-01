#include "Forest.h"
#include "../Utils/FileUtils.h"
#include "../Scenes/BattleScene.h"

namespace UCG {

	void Forest::Initialize(Flora::Scene* context, Flora::Entity tile) {
		SetType(BuildingType::FOREST);
		m_Context = (BattleScene*)context;
		m_TileBody = tile;
		m_Body = m_Context->CreateEntity("Forest Building");
		m_Body.AddComponent<Flora::ParentComponent>().Parent = tile;
		Flora::SpriteRendererComponent& src = m_Body.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& tc = m_Body.GetComponent<Flora::TransformComponent>();
		tc.Translation.z = 0.0001f;
		if (!tile.HasComponent<Flora::ChildComponent>()) tile.AddComponent<Flora::ChildComponent>();
		tile.GetComponent<Flora::ChildComponent>().AddChild(m_Body);
		src.Path = FileUtils::Path("assets/Tiles/Forest.png");
		AddWeakness({DamageTypes::FIRE, 3.0});
	}

}
