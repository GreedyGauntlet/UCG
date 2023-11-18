#pragma once
#include "Monster.h"

namespace UCG {
	class Slime : public Monster {
	public:
		virtual void Initialize(Flora::Scene* context, Flora::Entity tile) override;
		virtual void Update(Flora::Timestep ts) override;
	protected:
		virtual void DeathAnim(Flora::Timestep ts) override;
	};
}