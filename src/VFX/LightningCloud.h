#pragma once
#include "VFX.h"

namespace UCG {
	class LightningCloud : public VFX {
	public:
		virtual void Initialize(BattleScene* scene, TileRef tile) override;
		virtual bool Update() override;
		virtual bool Activate() override;
	};
}
