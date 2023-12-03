#pragma once
#include "GameScene.h"
#include "../Utils/BoardUtils.h"
#include "../Core/Player.h"
#include "../Core/CardDictionary.h"
#include "../Monsters/Monster.h"

namespace UCG {

	typedef uint64_t TileSelectFlag;

	enum TileSelectFlags {
		DIRT       = 1 << 0,
		FOREST     = 1 << 1,
		MOUNTAIN   = 1 << 2,
		PLAYER     = 1 << 3,
		OPPONENT   = 1 << 4,
		NEXUS      = 1 << 5,
		WATER      = 1 << 6,
		OCCUPIED   = 1 << 7,
		UNOCCUPIED = 1 << 8,
		ALL        = 1 << 9,
	};

	enum class BattleState {
		PREPLAYER,
		PREOPPONENT,
		PLAYER,
		ENDPLAYER,
		OPPONENT,
		ENDOPPONENT,
		POSTPLAYER,
		POSTOPPONENT,
	};

	struct PlayerUI {
		Flora::Entity ProfileBorder;
		Flora::Entity ProfilePicture;
		std::vector<Flora::Entity> ManaVessel;
		std::vector<Flora::Entity> HealthVessel;
	};

	struct BattleUI {
		PlayerUI PUI;
	};

	struct BattleStats {
		int PlayerHealth = 20;
		int PlayerMana = 28;
	};

	class BattleScene : public GameScene {
	public:
		virtual void Start() override;
		virtual void Update(Flora::Timestep ts) override;
		virtual void Stop() override;
	public:
		std::vector<std::vector<TileObj>> GetBoardTiles() { return m_BoardTiles; }
	private:
		void CreateUI();
		void UpdateUI();
	private:
		void UpdateMonsters(Flora::Timestep ts);
	private:
		void DrawHand();
		void DrawCard();
		bool ActivateCard(int cardIndex);
		void ConsumeCard();
		void UpdateHand(Flora::Timestep ts, bool fast = false);
		void UpdateSpell();
	private:
		void ResetBoard(const Board board);
		void CleanBoard();
		void DeleteBoard();
		void UpdateBoard(Flora::Timestep ts);
		bool TileCollision(Flora::Entity tile, glm::vec2 translation);
	private:
		std::vector<TileObj> NexusWorkingSet(bool playernexus = true, int radius = 1);
		bool ValidBoardCoord(int r, int c);
	private:
		void UpdateBattleState(Flora::Timestep ts);
	private:
		bool CheckHovered(Flora::Entity entity);
		void DevCall();
	private:
		template<typename SelectFunction>
		bool SelectTile(bool trigger, std::vector<TileObj> workingset, SelectFunction selectfunc, TileSelectFlag flags);
	private:
		std::vector<std::vector<TileObj>> m_BoardTiles;
		std::vector<std::pair<Flora::Entity, Card>> m_Hand;
		std::vector<Monster*> m_Monsters;
		BattleUI m_UI;
		BattleStats m_Stats;
		BattleState m_State = BattleState::PREPLAYER;
		int m_SelectedCard = -1;
		CardID m_CurrentSpell = CardID::NONE;
		int m_CurrentSpellCardIndex = -1;
	private:
		Flora::Entity m_PlayerNexus;
		Flora::Entity m_OpponentNexus;
	};
}