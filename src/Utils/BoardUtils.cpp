#include "BoardUtils.h"
#include "flpch.h"
#include "FileUtils.h"

namespace UCG {

	TileObj BoardUtils::Tile(Flora::Scene* context, char id) {
		std::string tilename = " ";
		tilename[0] = id;
		Flora::Entity tile = context->CreateEntity(tilename);
		Building* building = new Building();
		building->SetType(BuildingType::EMPTY);
		Flora::SpriteRendererComponent& src = tile.AddComponent<Flora::SpriteRendererComponent>();
		switch (id) {
		case 'D':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Dirt.png");
			break;
		case 'P':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Player.png");
			building->SetType(BuildingType::NEXUS);
			break;
		case 'O':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Opponent.png");
			building->SetType(BuildingType::NEXUS);
			break;
		case 'F':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Forest.png");
			building->SetType(BuildingType::FOREST);
			break;
		case 'M':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Mountain.png");
			building->SetType(BuildingType::MOUNTAIN);
			break;
		case 'W':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Water.png");
			break;
		default:
			src.Path = "invalid";
			break;
		}
		building->Initialize((GameScene*)context, tile);
		tile.AddComponent<Flora::ChildComponent>().AddChild(building->Body());
		return { building, tile };
	}

}