#pragma once
#include "Flora.h"
#include "Flora/Scene/Scene.h"

namespace UCG {
	class Monster {
	public:
		virtual ~Monster() = default;
		virtual void Initialize(Flora::Scene* context, Flora::Entity tile) = 0;
		virtual void Update(Flora::Timestep ts) = 0;
		virtual void Destroy();
		virtual void Damage(int damage);
	public:
		Flora::Entity Body() { return m_Body; }
		Flora::Entity Tile() { return m_Tile; }
		bool Alive() { return !m_Dead; }
	protected:
		virtual void DamageAnim(Flora::Timestep ts);
		virtual void DeathAnim(Flora::Timestep ts);
	protected:
		Flora::Scene* m_Context;
		Flora::Entity m_Body;
		Flora::Entity m_Tile;
	protected:
		float m_LocalTime = 0.0f;
		bool m_Damaged = false;
		bool m_Dead = false;
		int m_MaxHealth = 1;
		int m_Health = 1;
	};
}