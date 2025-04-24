#pragma once

#include "Pathfinder.h"
#include "CMapIterator.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <map>


inline int depthFirst(CMapIterator& it, const GridCoordinate A, const GridCoordinate B, std::vector<GridCoordinate>& Aset)
{
	// step 1: init
	if (!it.jumpToPosition(B)) return -1;		// B is not reachable
	if (!it.jumpToPosition(A)) return -1;		// A is not reachable
	std::vector<GridCoordinate> Mset;			// set of marked nodes
	Mset.push_back(A);
	Aset.clear();
	Aset.push_back(A);

	while (it.getPosition() != B) {
		// step 2: search
		bool stepDone = false;
		for (int i = 0; i < 4 && !stepDone; i++) {
			switch (i) {
				case 0: stepDone = it.moveNorth(); break;
				case 1: stepDone = it.moveEast(); break;
				case 2: stepDone = it.moveSouth(); break;
				case 3: stepDone = it.moveWest();
				default: break;
			}
			
			if (stepDone) {
				// New node is marked already 
				if (std::ranges::find(Mset, it.getPosition()) != Mset.end()) {
					stepDone = false;
					it.moveBack();
				}
				// New node is not in the set of marked nodes
				else {
					Mset.push_back(it.getPosition());
					Aset.push_back(it.getPosition());
				}
			}
		}

		// There is no step to an unmarked node
		if (!stepDone) {
			Aset.pop_back();
			if (!Aset.empty())
				it.jumpToPosition(Aset.back());
			else
				return 0;
		}
	}
	return static_cast<int>(Aset.size());
}

//TODO: Mal gucken ob das tut, irrelevant für die Aufgabe. Mir war ein wenig langweilig
inline int aStar(CMapIterator& it, const GridCoordinate A, const GridCoordinate B, std::vector<GridCoordinate>& path)
{
    // Check if start and end positions are valid
    if (!it.jumpToPosition(B)) return -1;     // B is not reachable
    if (!it.jumpToPosition(A)) return -1;     // A is not reachable

    // Clear the path
    path.clear();

    // Open list - nodes to be evaluated
    std::vector<GridCoordinate> openList;
    openList.push_back(A);

    // Maps for tracking (using std::map instead of unordered_map to avoid hash requirements)
    std::map<GridCoordinate, GridCoordinate> cameFrom;    // To reconstruct path
    std::map<GridCoordinate, float> gScore;               // Cost from start to node
    std::map<GridCoordinate, float> fScore;               // Estimated total cost
    std::vector<GridCoordinate> closedList;               // Nodes already evaluated

    // Initialize start node
    gScore[A] = 0.0f;
    fScore[A] = static_cast<float>(std::abs(A.x - B.x) + std::abs(A.y - B.y));  // Manhattan distance heuristic

    while (!openList.empty()) {
        // Find node with lowest fScore in openList
        auto current_it = openList.begin();
        for (auto wrap_iter = openList.begin(); wrap_iter != openList.end(); ++wrap_iter) {
            if (fScore[*wrap_iter] < fScore[*current_it]) {
                current_it = wrap_iter;
            }
        }

        // Get current node and remove from open list
        GridCoordinate current = *current_it;
        openList.erase(current_it);

        // If we reached the goal
        if (current == B) {
            // Reconstruct path
            path.push_back(current);
            while (current != A) {
                current = cameFrom[current];
                path.insert(path.begin(), current);
            }
            return static_cast<int>(path.size());
        }

        // Add current to closed list
        closedList.push_back(current);

        // Jump to current position
        it.jumpToPosition(current);

        // Check all four directions
        for (int dirIndex = 0; dirIndex < 4; ++dirIndex) {
            bool canMove = false;

            // Try to move in the current direction
            switch (dirIndex) {
                case 0: canMove = it.moveNorth(); break;
                case 1: canMove = it.moveEast(); break;
                case 2: canMove = it.moveSouth(); break;
                case 3: canMove = it.moveWest(); break;
	            default: throw std::invalid_argument("Invalid direction");
            }

            if (canMove) {
                GridCoordinate neighbor = it.getPosition();

                // Skip if neighbor is in closed list
                if (std::ranges::find(closedList, neighbor) != closedList.end()) {
                    it.moveBack();
                    continue;
                }

                // Calculate tentative gScore (cost = 1 for each step)
                const float tentativeGScore = gScore[current] + 1.0f;

                if (const bool neighborInOpenList = std::ranges::find(openList, neighbor) != openList.end(); !neighborInOpenList || tentativeGScore < gScore[neighbor]) {
                    // Record this path
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentativeGScore;
                    fScore[neighbor] = tentativeGScore + static_cast<float>(std::abs(neighbor.x - B.x)) + static_cast<float>(std::abs(neighbor.y - B.y));

                    // Add to open list if not already there
                    if (!neighborInOpenList) {
                        openList.push_back(neighbor);
                    }
                }

                // Move back for next direction check
                it.moveBack();
            }
        }
    }

    // No path found
    return 0;
}