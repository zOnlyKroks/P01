#pragma once

#include "Pathfinder.h"
#include "CMapIterator.h"
#include <vector>
#include <algorithm>
#include <map>
#include <set>

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

inline int aStarSearch(CMapIterator& it, const GridCoordinate A, const GridCoordinate B, std::vector<GridCoordinate>& path)
{
    if (!it.jumpToPosition(B)) return -1;  // B is not reachable
    if (!it.jumpToPosition(A)) return -1;  // A is not reachable

    // Clear output path
    path.clear();

    // (Manhattan distance)
    auto heuristic = [](const GridCoordinate& a, const GridCoordinate& b) -> int {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    };

    // Use map instead of unordered_map since GridCoordinate doesn't have a hash
    std::map<GridCoordinate, GridCoordinate> came_from;
    std::map<GridCoordinate, int> g_score;

    // Set of visited nodes
	std::set<GridCoordinate, GridCoordinate> closed_set;

    struct CompareCoord {
        std::map<GridCoordinate, int>& f_scores;

        explicit CompareCoord(std::map<GridCoordinate, int>& scores) : f_scores(scores) {}

        bool operator()(const GridCoordinate& a, const GridCoordinate& b) const {
            return f_scores[a] > f_scores[b];  // Min-heap needs reverse comparison
        }
    };

    std::map<GridCoordinate, int> f_score;
    f_score[A] = heuristic(A, B);
    g_score[A] = 0;

    std::vector<GridCoordinate> open_set;
    open_set.push_back(A);

    while (!open_set.empty()) {
        // Find node with lowest f_score in open set
        auto current_it = std::ranges::min_element(open_set,
                                                   [&f_score](const GridCoordinate& a, const GridCoordinate& b) {
	                                                   return f_score[a] < f_score[b];
                                                   });

        GridCoordinate current = *current_it;

        // Remove from open set
        open_set.erase(current_it);

        // Check if we reached the goal
        if (current == B) {
            // Reconstruct path
            path.clear();
            while (current != A) {
                path.push_back(current);
                current = came_from[current];
            }
            path.push_back(A);

            // Reverse to get path from A to B
            std::ranges::reverse(path);
            return static_cast<int>(path.size());
        }

        // Add to closed set
        closed_set.insert(current);

        // Jump to current position
        it.jumpToPosition(current);

        // Check all neighbors
        for (int dir = 0; dir < 4; dir++) {
        	// Direction mappings for readability
	        constexpr int NORTH = 0;
        	constexpr int EAST = 1;
        	constexpr int SOUTH = 2;
	        constexpr int WEST = 3;

	        bool moved = false;

            // Try to move in each direction
            switch (dir) {
                case NORTH: moved = it.moveNorth(); break;
                case EAST:  moved = it.moveEast();  break;
                case SOUTH: moved = it.moveSouth(); break;
                case WEST:  moved = it.moveWest();  break;
	            default: break;
            }

            if (moved) {
                GridCoordinate neighbor = it.getPosition();

                // Move back to current position
                it.moveBack();

                // Skip if neighbor is in closed set
                if (closed_set.contains(neighbor)) {
                    continue;
                }

                // Calculate tentative g_score
                const int tentative_g_score = g_score[current] + 1;

                // Check if we found a better path
                bool is_better = false;
                if (!g_score.contains(neighbor)) {
                    // Neighbor not evaluated yet
                    is_better = true;
                    // Add to open set if not there
                    if (std::ranges::find(open_set, neighbor) == open_set.end()) {
                        open_set.push_back(neighbor);
                    }
                } else if (tentative_g_score < g_score[neighbor]) {
                    // We found a better path
                    is_better = true;
                }

                if (is_better) {
                    // Record this better path
                    came_from[neighbor] = current;
                    g_score[neighbor] = tentative_g_score;
                    f_score[neighbor] = tentative_g_score + heuristic(neighbor, B);
                }
            }
        }
    }

    // No path found
    return 0;
}