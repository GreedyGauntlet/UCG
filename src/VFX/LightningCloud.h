#pragma once
#include "Flora.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "../Scenes/BattleScene.h"


namespace UCG {
	class LightningCloud {
	public:
		LightningCloud(BattleScene*, Flora::Entity tile);
		Flora::Entity Tile() { return m_Tile; }
		bool Update();
		bool Activate();
	private:
		Flora::Entity m_VFX;
		Flora::Entity m_Tile;
		BattleScene* m_Scene;
		bool m_Active = false;
	};
}