#pragma once
#include "Building.h"

namespace UCG {
	enum class NexusAlignment {
		PLAYER,
		OPPONENT,
	};

	class Nexus : public Building {
	public:
		Nexus(NexusAlignment alignment);
		virtual void Initialize(Flora::Scene* context, Flora::Entity tile) override;
	private:
		NexusAlignment m_Alignment;
	};
}
