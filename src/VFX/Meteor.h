#pragma once
#include "VFX.h"

namespace UCG {
	class Meteor : public VFX {
	public:
		virtual void Initialize(BattleScene* scene, TileRef tile) override;
		virtual bool Update() override;
		virtual bool Activate() override;
	};
}
