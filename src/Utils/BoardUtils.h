#pragma once
#include <vector>
#include <string>
#include "Flora.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scene/Components.h"
#include "../Buildings/Building.h"

namespace UCG {
	typedef std::vector<std::vector<char>> Board;
	typedef std::pair<Building*, Flora::Entity> TileObj;

	class BoardUtils {
	public:
		static TileObj Tile(Flora::Scene* context, char id);
	};
}
