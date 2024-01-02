#pragma once
#include "Monster.h"

namespace UCG {
	class Goblin : public Monster {
	public:
		virtual void Initialize(GameScene* context, Flora::Entity tile) override;
		virtual void StartTurn() override;
		virtual void Attack() override;
	};
}
