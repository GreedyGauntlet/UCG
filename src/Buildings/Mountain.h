#pragma once
#include "Building.h"

namespace UCG {
	class Mountain : public Building {
	public:
		virtual void Initialize(Flora::Scene* context, Flora::Entity tile) override;
	};
}
