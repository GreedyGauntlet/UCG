#include "CardDictionary.h"

namespace UCG {

	Card CardDictionary::GetCard(CardID ID) {
		Card card;
		card.ID = ID;
		switch (ID) {
		case CardID::SMITE:
			card.Cost = 2;
			card.Name = "Smite";
			card.Description = "Deals 1 damage to any tile";
			break;
		case CardID::GOBLIN:
			card.Cost = 3;
			card.Name = "Goblin";
			card.Description = "Summons an aggressive goblin with 5 health and 1 melee damage to any adjacent tiles. Roams randomly 1 tile every other turn.";
			break;
		case CardID::SLIME:
			card.Cost = 1;
			card.Name = "Slime";
			card.Description = "Summons a slime with 3 health and 0 attack that does nothing. However, this slime will revive up to 5 times upon death after 2 turns.";
			break;
		case CardID::METEOR:
			card.Cost = 5;
			card.Name = "Meteor";
			card.Description = "Summons a destructive metor that can even damage the terrain. Does 5 damage to monsters and 1 damage to terrain.";
			break;
		case CardID::SANCTUARY:
			card.Cost = 2;
			card.Name = "Sanctuary";
			card.Description = "Turns a given tile into a sanctuary tile for 3 turns. Any monster summoned on this tile will be allied with the player.";
			break;
		default:
			break;
		}
		return card;
	}
}