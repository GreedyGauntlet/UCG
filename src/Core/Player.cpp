#include "Player.h"

namespace UCG {

	static Flora::Ref<Deck> s_Deck;

	static int tempdeck[40] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		2, 3, 0, 2, 3, 1, 1, 1, 3, 2
	};

	Flora::Ref<Deck> Player::CreateDeck() {
		s_Deck = Flora::CreateRef<Deck>();

		for (int i = 0; i < 40; i++)
			s_Deck->Cards.push_back(CardDictionary::GetCard((CardID)tempdeck[i]));

		return s_Deck;
	}

	Flora::Ref<Deck> Player::GetDeck() {
		if (!s_Deck) return CreateDeck();
		return s_Deck;
	}

}