#pragma once
#include "Monster.h"
#include "../Scenes/BattleScene.h"

namespace UCG {
	enum ObjectSelectFlags {
		FRIENDLYFIRE   = 1 << 0,
		NEAR           = 1 << 1,
		FAR            = 1 << 2,
		LINEAR         = 1 << 3,
		DIAGONAL       = 1 << 4,
    	DIRECTIONAL    = 1 << 5,
		EMPTY          = 1 << 6,
	};

	class Behaviors {
	public:
		static CoordinateSet Target(BattleScene* context, Monster& subject, ObjectSelectFlags flags, uint32_t numTargets = 1, const CoordinateSet& included = CoordinateSet());
	};
}
