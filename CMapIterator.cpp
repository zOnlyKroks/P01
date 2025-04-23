// CMapIterator.cpp
#include "CMapIterator.h"

CMapIterator::CMapIterator(std::vector<std::vector<GridTile>>& m) : map(m)
{
    posInternal.x = -1;
    posInternal.y = -1;
    lastPos = posInternal;
}

GridCoordinate CMapIterator::getPosition() const {
    return posInternal;
}

bool CMapIterator::jumpToPosition(const GridCoordinate& pos)
{
    // Check if position is within map bounds
    if (pos.y < 0 || pos.y >= map.size() || pos.x < 0 || pos.x >= map[pos.y].size())
        return false;

    // Check if the target position is a trail (not an obstacle)
    if (map[pos.y][pos.x].type != TileType::trail)
        return false;

    // Store current position as last position
    lastPos = posInternal;

    // Update current position
    posInternal = pos;

    return true;
}

void CMapIterator::moveBack()
{
    posInternal = lastPos;
}

bool CMapIterator::moveNorth()
{
    GridCoordinate newPos = posInternal;
    newPos.y--;

    if (newPos.y < 0 || newPos.y >= map.size() ||
        newPos.x < 0 || newPos.x >= map[newPos.y].size() ||
        map[newPos.y][newPos.x].type != TileType::trail)
        return false;

    lastPos = posInternal;
    posInternal = newPos;
    return true;
}

bool CMapIterator::moveSouth()
{
    GridCoordinate newPos = posInternal;
    newPos.y++;

    if (newPos.y < 0 || newPos.y >= map.size() ||
        newPos.x < 0 || newPos.x >= map[newPos.y].size() ||
        map[newPos.y][newPos.x].type != TileType::trail)
        return false;

    lastPos = posInternal;
    posInternal = newPos;
    return true;
}

bool CMapIterator::moveWest()
{
    GridCoordinate newPos = posInternal;
    newPos.x--;

    if (newPos.y < 0 || newPos.y >= map.size() ||
        newPos.x < 0 || newPos.x >= map[newPos.y].size() ||
        map[newPos.y][newPos.x].type != TileType::trail)
        return false;

    lastPos = posInternal;
    posInternal = newPos;
    return true;
}

bool CMapIterator::moveEast()
{
    GridCoordinate newPos = posInternal;
    newPos.x++;

    if (newPos.y < 0 || newPos.y >= map.size() ||
        newPos.x < 0 || newPos.x >= map[newPos.y].size() ||
        map[newPos.y][newPos.x].type != TileType::trail)
        return false;

    lastPos = posInternal;
    posInternal = newPos;
    return true;
}