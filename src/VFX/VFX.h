#pragma once
#include "Flora.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "../Scenes/BattleScene.h"

namespace UCG {
	class VFX {
	public:
		virtual ~VFX() = default;
		virtual void Initialize(BattleScene* scene, Flora::Entity tile);
		virtual bool Update() { return false; };
		virtual bool Activate() { return false; };
	public:
		Flora::Entity Tile() { return m_Tile; }
	protected:
		Flora::Entity m_VFX;
		Flora::Entity m_Tile;
		BattleScene* m_Scene;
		bool m_Active = false;
	};
}
