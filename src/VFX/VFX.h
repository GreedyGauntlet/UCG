#pragma once
#include "Flora.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "../Scenes/BattleScene.h"

namespace UCG {
	class VFX {
	public:
		virtual ~VFX() = default;
		virtual void Initialize(BattleScene* scene, TileRef tile);
		virtual bool Update() { return false; };
		virtual bool Activate() { return false; };
		virtual bool TurnUpdate() { return false; };
		virtual void PersistantUpdate() {};
		virtual void Kill();
	public:
		Tile GetTile() { return m_Scene->GetTile(m_Tile); }
	protected:
		Flora::Entity m_VFX;
		TileRef m_Tile;
		BattleScene* m_Scene;
		bool m_Active = false;
	};
}
