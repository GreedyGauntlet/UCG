#pragma once
#include "VFX.h"

namespace UCG {
	class Sanctuary : public VFX {
	public:
		virtual void Initialize(BattleScene* scene, TileRef tile) override;
		virtual bool TurnUpdate() override;
		virtual void PersistantUpdate() override;
	private:
    	bool m_PlayerAligned = true;
		int m_Health = 5;
	};
}
