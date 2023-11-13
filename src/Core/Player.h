#pragma once
#include "Flora.h"

namespace UCG {
	struct Card {
		std::string Name = "";
		std::string Description = "";
		int64_t ID = -1;
		int Cost = 0;
	};

	struct Deck {
		std::vector<Card> Cards;
		Card Draw() {
			Card card = Cards.back();
			Cards.pop_back();
			return card;
		}
	};

	class Player {
	public:
		static Flora::Ref<Deck> CreateDeck();
		static Flora::Ref<Deck> GetDeck();
	};
}