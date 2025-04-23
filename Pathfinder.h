#pragma once
#include <iostream>

enum class TileType {
	undefined,
	trail,
	obstacle
};

struct GridTile {
	TileType type;
	int cost;
};

struct GridCoordinate {
	int x;
	int y;

	/* output GridCoordinate --- if needed */
	friend std::ostream& operator<< (std::ostream& os, const GridCoordinate& c) {
		os << c.x << "/" << c.y;
		return os;
	}

	bool operator==(const GridCoordinate& other) const {
		return x == other.x && y == other.y;
	}
};