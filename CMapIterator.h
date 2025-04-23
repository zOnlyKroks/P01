// CMapIterator.h
#pragma once
#include <vector>
#include "Pathfinder.h"

class CMapIterator {
    GridCoordinate posInternal{};
    GridCoordinate lastPos{};
    std::vector<std::vector<GridTile>>& map;

public:
    explicit CMapIterator(std::vector<std::vector<GridTile>>& m);
    [[nodiscard]] GridCoordinate getPosition() const;
    bool jumpToPosition(const GridCoordinate& pos);
    void moveBack();
    bool moveNorth();
    bool moveSouth();
    bool moveWest();
    bool moveEast();
};