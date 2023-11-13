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
		void DeleteBoard();
		void UpdateBoard();
		bool TileCollision(Flora::Entity tile, glm::vec2 translation);
	private:
		std::vector<std::vector<Flora::Entity>> m_BoardEntities;
		Flora::Entity m_Cursor;
	};
}