#include "BoardUtils.h"

namespace UCG {

	std::string BoardUtils::TilePath(char id) {
		switch (id) {
		case 'D':
			return "assets/Tiles/Dirt.png";
		case 'P':
			return "assets/Tiles/Player.png";
		case 'O':
			return "assets/Tiles/Opponent.png";
		case 'F':
			return "assets/Tiles/Forest.png";
		case 'M':
			return "assets/Tiles/Mountain.png";
		case 'W':
			return "assets/Tiles/Water.png";
		default:
			return "invalid";
		}
	}

}