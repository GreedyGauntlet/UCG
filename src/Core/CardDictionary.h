#pragma once
#include <string>

namespace UCG {
	enum class CardID {
		NONE = -1,
		SMITE = 0,
		GOBLIN = 1,
		SLIME = 2,
		METEOR = 3,
	};

	struct Card {
		std::string Name = "";
		std::string Description = "";
		CardID ID = CardID::NONE;
		int Cost = 0;
	};

	class CardDictionary {
	public:
		static Card GetCard(CardID ID);
	};
}