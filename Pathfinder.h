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

	friend std::ostream& operator<< (std::ostream& os, const GridCoordinate& c) {
		os << c.x << "/" << c.y;
		return os;
	}

	bool operator<(const GridCoordinate& other) const {
		return std::tie(x, y) < std::tie(other.x, other.y);
	}

	bool operator==(const GridCoordinate& other) const {
		return x == other.x && y == other.y;
	}

	bool operator()(const GridCoordinate& a, const GridCoordinate& b) const {
		return std::tie(a.x, a.y) < std::tie(b.x, b.y);
	}
};