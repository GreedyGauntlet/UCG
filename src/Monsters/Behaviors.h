#pragma once
#include "Monster.h"
#include "../Scenes/BattleScene.h"

namespace UCG {
	typedef std::vector<TileRef> TileSet;

	enum ObjectSelectFlags {
		FRIENDLYFIRE   = 1 << 0,
		NEAR           = 1 << 1,
		FAR            = 1 << 2,
		LINEAR         = 1 << 3,
		DIAGONAL       = 1 << 4,
    	DIRECTIONAL    = 1 << 5,
	};

	class Behaviors {
	public:
		static TileSet Target(BattleScene* context, Monster& subject, ObjectSelectFlags flags, uint32_t numTargets = 1, const TileSet& included = TileSet());
	};
}
