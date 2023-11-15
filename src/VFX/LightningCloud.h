#pragma once
#include "Flora.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "../Scenes/BattleScene.h"


namespace UCG {
	class LightningCloud {
	public:
		LightningCloud(BattleScene*, Flora::Entity tile);
		bool Update();
	private:
		Flora::Entity m_VFX;
		BattleScene* m_Scene;
	};
}