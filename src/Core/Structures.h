#pragma once
#include "Flora/Scene/Entity.h"

namespace UCG {
	
	class Building;

	enum class TileTypes {
		DIRT,
		WATER,
	};

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

	struct TileObj {
		Building* Physical;
		Flora::Entity Body;
	};

	struct Tile	{
		TileRef Coordinates;
		TileObj Contents;
		TileTypes Type;
	};

	typedef std::vector<Tile> TileSet;
	typedef std::vector<TileRef> CoordinateSet;
	typedef std::vector<std::vector<char>> Blueprint;
	typedef std::vector<std::vector<Tile>> Board;

	struct Map {
		Blueprint Terrain;
		Blueprint Buildings;
	};

}
