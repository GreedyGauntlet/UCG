#pragma once
#include "GameScene.h"
#include "../Core/Structures.h"
#include "../Core/Player.h"
#include "../Core/CardDictionary.h"

namespace UCG {

	typedef uint64_t TileSelectFlag;

	class VFX;
	class Monster;

	struct BoardObjects {
		TileRef PlayerNexus;
		TileRef OpponentNexus;
		Board Tiles;
		std::vector<Monster*> Monsters;
		std::vector<VFX*> VFXs;
	};

	struct Hand {
		std::vector<std::pair<Flora::Entity, Card>> Cards;
		int SelectedCard = -1;
		CardID CurrentSpell = CardID::NONE;
		int CurrentSpellCardIndex = -1;
	};

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
		BattleState State = BattleState::PREPLAYER;
		int PlayerHealth = 20;
		int PlayerMana = 28;
	};

	class BattleScene : public GameScene {
	public:
		virtual void Start() override;
		virtual void Update(Flora::Timestep ts) override;
		virtual void Stop() override;
	public:
		Board GetTiles() { return m_BoardObjects.Tiles; }
		bool ValidBoardCoord(TileRef tile);
		bool ValidBoardCoord(Tile tile) { return ValidBoardCoord(tile.Coordinates); }
		bool TileOccupied(TileRef tile);
		bool TileOccupied(Tile tile) { return TileOccupied(tile.Coordinates); }
		Monster* GetMonster(TileRef tile);
		Monster* GetMonster(Tile tile) { return GetMonster(tile.Coordinates); }
		Monster* GetTarget(TileRef tile);
		Monster* GetTarget(Tile tile) { return GetTarget(tile.Coordinates); }
		TileRef GetTileRef(Flora::Entity tile);
		Tile GetTile(TileRef tile) { return m_BoardObjects.Tiles[tile.r][tile.c]; }
		Tile GetTile(Flora::Entity tile) { return GetTile(GetTileRef(tile)); }
		TileRef GetPlayerNexus() { return m_BoardObjects.PlayerNexus; }
		TileRef GetOpponentNexus() { return m_BoardObjects.OpponentNexus; }
	private:
		void CreateUI();
		void UpdateUI();
	private:
		void UpdateBoardObjects(Flora::Timestep ts);
	private:
		void DrawHand();
		void DrawCard();
		bool ActivateCard(int cardIndex);
		void ConsumeCard();
		void UpdateHand(Flora::Timestep ts, bool fast = false);
		void UpdateSpell();
	private:
		void DeepTint(Flora::Entity tile, glm::vec4 color);
		void ResetBoard(const Map map);
		void CleanBoard();
		void DeleteBoard();
		void UpdateBoard(Flora::Timestep ts);
		bool TileCollision(Flora::Entity tile, glm::vec2 translation);
		Tile MakeTile(TileRef coordinate, Map map);
	private:
		std::vector<TileObj> NexusWorkingSet(bool playernexus = true, int radius = 2);
	private:
		void UpdateBattleState(Flora::Timestep ts);
	private:
		bool CheckHovered(Flora::Entity entity);
		void DevCall();
	private:
		template<typename SelectFunction>
		bool SelectTile(bool trigger, std::vector<TileObj> workingset, SelectFunction selectfunc, TileSelectFlag flags);
	private:
		BoardObjects m_BoardObjects;
		Hand m_Hand;
		BattleUI m_UI;
		BattleStats m_Stats;
	};
}
