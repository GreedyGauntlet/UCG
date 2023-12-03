#include "RandomUtils.h"
#include <random>

namespace UCG {

	int RandomUtils::Random(int min, int max) {
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(generator);
	}

}