#pragma once
#include "VFX.h"

namespace UCG {
	class Sanctuary : public VFX {
	public:
		virtual void Initialize(BattleScene* scene, Flora::Entity tile) override;
		virtual bool TurnUpdate() override;
		virtual void PersistantUpdate() override;
	private:
		int m_Health = 5;
		std::pair<int, int> m_TileCoords;
	};
}