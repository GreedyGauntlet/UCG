#pragma once
#include "Flora.h"
#include "CardDictionary.h"

namespace UCG {
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