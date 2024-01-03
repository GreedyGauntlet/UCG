#include "VFX.h"

namespace UCG {

	void VFX::Initialize(BattleScene* scene, TileRef tile)  {
		m_Tile = tile;
		m_Scene = scene;
	}

	void VFX::Kill() {
		m_Scene->DestroyEntity(m_VFX);
	}

}
