#include "Player.h"

namespace UCG {

	static Flora::Ref<Deck> s_Deck;

	Flora::Ref<Deck> Player::CreateDeck() {
		s_Deck = Flora::CreateRef<Deck>();

		for (int i = 0; i < 40; i++) {
			Card card;
			card.Cost = 5;
			card.Name = "Example Card";
			card.Description = "Does literally nothing";
			card.ID = 0;
			s_Deck->Cards.push_back(card);
		}

		return s_Deck;
	}

	Flora::Ref<Deck> Player::GetDeck() {
		if (!s_Deck) return CreateDeck();
		return s_Deck;
	}

}