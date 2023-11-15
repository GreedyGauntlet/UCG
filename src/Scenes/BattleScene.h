#pragma once
#include "GameScene.h"
#include "../Utils/BoardUtils.h"
#include "../Core/Player.h"
#include "../Core/CardDictionary.h"

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
		int PlayerMana = 28;
	};

	class BattleScene : public GameScene {
	public:
		virtual void Start() override;
		virtual void Update(Flora::Timestep ts) override;
		virtual void Stop() override;
	private:
		void CreateUI();
		void UpdateUI();
	private:
		void DrawHand();
		void DrawCard();
		bool ActivateCard(int cardIndex);
		void UpdateHand(Flora::Timestep ts, bool fast = false);
		void UpdateSpell();
	private:
		void ResetBoard(const Board board);
		void CleanBoard();
		void DeleteBoard();
		void UpdateBoard();
		bool TileCollision(Flora::Entity tile, glm::vec2 translation);
	private:
		void UpdateBattleState();
	private:
		bool CheckHovered(Flora::Entity entity);
		void DevCall();
	private:
		std::vector<std::vector<Flora::Entity>> m_BoardEntities;
		std::vector<std::pair<Flora::Entity, Card>> m_Hand;
		BattleUI m_UI;
		BattleStats m_Stats;
		BattleState m_State = BattleState::PREPLAYER;
		int m_SelectedCard = -1;
		CardID m_CurrentSpell = CardID::NONE;
	};
}