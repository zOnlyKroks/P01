#define CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "SearchAlgorithm.h"
#include "CGridtypeMap.h"
#include "CMapIterator.h"

int main() {
    const std::string filename = "map03.CSV";

    // Test 1: Import map and show dimensions
    CGridtypeMap map;
    map.importMap(filename);
    std::cout << "WIDTH: " << map.mapWidth() << "\t HEIGHT: " << map.mapHeight() << "\n";

    // Test 3: Test iterator
    system("pause");
    CMapIterator it = map.leapIn(GridCoordinate{ 1, 1 });
    std::cout << "Current position of CMapIterator object: " << it.getPosition() << "\n";

    system("pause");
    std::cout << "Map (without path): \n" << std::endl;

    map.toConsole(true, {});

    // Test 4: Find path
    std::vector<GridCoordinate> pathCoordinates;
    GridCoordinate start{}, goal{};
    start.x = 3;
    start.y = 9;
    goal.x = 9;
    goal.y = 4;

    system("pause");

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