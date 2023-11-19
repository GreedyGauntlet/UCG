#include "VFX.h"

namespace UCG {

	void VFX::Initialize(BattleScene* scene, Flora::Entity tile)  {
		m_Tile = tile;
		m_Scene = scene;
	}

}