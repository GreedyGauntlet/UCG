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
	protected:
		Flora::Scene* m_Context;
		Flora::Entity m_Body;
	};
}