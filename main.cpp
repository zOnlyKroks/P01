#define CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "CGridtypeMap.h"
#include "CMapIterator.h"
#include "SearchAlgorithm.h"

#ifdef __APPLE__
  #define PAUSE_CMD system("read -n 1 -s -p \"Press any key to continue...\"")
#else
  #define PAUSE_CMD system("pause")
#endif


int main() {
    const std::string filename = "map03.CSV";

    // Test 1: Import map and show dimensions
    CGridtypeMap map;
    map.importMap(filename);
    std::cout << "WIDTH: " << map.mapWidth() << "\t HEIGHT: " << map.mapHeight() << "\n";

    // Test 3: Test iterator
    PAUSE_CMD;
    CMapIterator it = map.leapIn(GridCoordinate{ 1, 1 });
    std::cout << "\nCurrent position of CMapIterator object: " << it.getPosition() << "\n";

    PAUSE_CMD;
    std::cout << "Map (without path): \n" << std::endl;

    map.toConsole(true, {});

    PAUSE_CMD;

    // Test 4: Find path
    std::vector<GridCoordinate> pathCoordinates;
    GridCoordinate start{}, goal{};
    start.x = 1;
    start.y = 11;
    goal.x = 5;
    goal.y = 1;

    // Display the path
    if (const int pathLen = depthFirst(it, start, goal, pathCoordinates); pathLen > 0) {
        std::cout << "Path found with " << pathLen << " steps.\n";

        std::cout << "Map (with path):\n";

        // Show the map with the path overlaid
        map.toConsole(true, pathCoordinates);

        // Print path coordinates
        std::cout << "\nPath coordinates:\n";
        for (size_t i = 0; i < pathCoordinates.size(); i++) {
            std::cout << i << ": " << pathCoordinates[i] << "\n";
        }
    }
    else {
        std::cout << "No path found.\n";
    }

    return EXIT_SUCCESS;
}