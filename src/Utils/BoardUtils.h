#pragma once
#include <vector>
#include <string>
#include "Flora.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scene/Components.h"

namespace UCG {
	typedef std::vector<std::vector<char>> Board;

	class BoardUtils {
	public:
		static Flora::Entity Tile(Flora::Scene* context, char id);
	};
}