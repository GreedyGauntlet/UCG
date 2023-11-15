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
			card.Cost = 1;
			card.Name = "Goblin";
			card.Description = "Summons a goblin with 1 health and does 1 damage to any adjacent tiles. Roams randomly 1 tile every other turn.";
			break;
		}
		return card;
	}
}