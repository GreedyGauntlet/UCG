#pragma once
#include <vector>
#include <string>

namespace UCG {
	typedef std::vector<std::vector<char>> Board;

	class BoardUtils {
	public:
		static std::string TilePath(char id);
	};
}