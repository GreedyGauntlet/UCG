#pragma once
#include "GameScene.h"
#include "../Utils/BoardUtils.h"

namespace UCG {
	class BattleScene : public GameScene {
	public:
		virtual void Start() override;
		virtual void Update(Flora::Timestep ts) override;
		virtual void Stop() override;
	private:
		void ResetBoard(const Board board);
	};
}