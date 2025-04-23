// CGridtypeMap.h
#pragma once
#include <vector>
#include <string>
#include "Pathfinder.h"
#include "CMapIterator.h"

class CGridtypeMap {
    std::vector<std::vector<GridTile>> map;
    CMapIterator it;

public:
    CGridtypeMap();
    [[nodiscard]] unsigned int mapWidth() const;
    [[nodiscard]] unsigned int mapHeight() const;
    int importMap(const std::string& fileName);
    void toConsole(bool hideCosts, const std::vector<GridCoordinate>& path);
    CMapIterator& leapIn(const GridCoordinate& pos);
};