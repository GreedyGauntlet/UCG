#pragma once
#include "Flora/Scene/Entity.h"

namespace UCG {
	struct TileRef {
		int r;
		int c;
		
		bool operator<(const TileRef& other) const {
        	return (r < other.r) || (r == other.r && c < other.c);
    	}

    	bool operator==(const TileRef& other) const {
        	return (r == other.r) && (c == other.c);
    	}
	};

	/*
	struct TileObj {
		Flora::Entity Building;
		Flora::Entity Body;
	};
/*
	struct Tile	{
		TileRef Coordinates;
		TileObj Contents;
	};

	typedef std::vector<> TileSet;
	typedef std::vector<std::vector<char>> Blueprint;
	typedef std::vector<std::vector<Tile>> Board*/
}
