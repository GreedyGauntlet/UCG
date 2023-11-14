#pragma once
#include "GameScene.h"
#include "../Utils/BoardUtils.h"
#include "../Core/Player.h"

namespace UCG {
	enum class BattleState {
		PREPLAYER,
		PREOPPONENT,
		PLAYER,
		OPPONENT,
		POSTPLAYER,
		POSTOPPONENT,
	};

	struct PlayerUI {
		Flora::Entity ProfileBorder;
		Flora::Entity ProfilePicture;
		std::vector<Flora::Entity> Health;
		std::vector<Flora::Entity> Mana;
	};

	struct BattleUI {
		PlayerUI PUI;
	};

	struct BattleStats {
		int PlayerHealth = 10;
		int PlayerMana = 8;
	};

	class BattleScene : public GameScene {
	public:
		virtual void Start() override;
		virtual void Update(Flora::Timestep ts) override;
		virtual void Stop() override;
	private:
		void CreateUI();
	private:
		void DrawHand();
		void DrawCard();
		void UpdateHand(bool fast = false);
	private:
		void ResetBoard(const Board board);
		void DeleteBoard();
		void UpdateBoard();
		bool TileCollision(Flora::Entity tile, glm::vec2 translation);
	private:
		void UpdateBattleState();
	private:
		void DevCall();
	private:
		std::vector<std::vector<Flora::Entity>> m_BoardEntities;
		std::vector<std::pair<Flora::Entity, Card>> m_Hand;
		BattleUI m_UI;
		BattleStats m_Stats;
		BattleState m_State = BattleState::PREPLAYER;
		int m_SelectedCard = -1;
	};
}