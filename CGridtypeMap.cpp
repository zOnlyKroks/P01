// CGridtypeMap.cpp
#include "CGridtypeMap.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <iomanip>

CGridtypeMap::CGridtypeMap() : it(map)
{
}

unsigned int CGridtypeMap::mapWidth() const {
    if (map.empty())
        return 0;

    unsigned int maxWidth = 0;
    for (const auto& row : map) {
        if (row.size() > maxWidth)
            maxWidth = row.size();
    }

    return maxWidth;
}

unsigned int CGridtypeMap::mapHeight() const {
    return map.size();
}

int CGridtypeMap::importMap(const std::string& fileName)
{
    std::ifstream f(fileName);
    if (f.fail()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return 0;
    }

    // Clear previous map
    map.clear();

    char bfr[1024];
    int line = 0;

    while (f.getline(bfr, sizeof(bfr))) {
        const auto delim = ",;.-";
        std::vector<GridTile> v;
        map.push_back(v);  // Add empty vector<GridTile>

        const char* token = std::strtok(bfr, delim);
        while (token != nullptr) {
            if (std::isdigit(*token))
                map[line].push_back(GridTile{ TileType::trail, std::stoi(token) });
            else
                map[line].push_back(GridTile{ TileType::obstacle, -1 });

            token = std::strtok(nullptr, delim);
        }
        line++;
    }

    std::cout << "Loaded map with " << line << " lines." << std::endl;
    return line;
}

void CGridtypeMap::toConsole(const bool hideCosts, const std::vector<GridCoordinate>& path)
{
    // ANSI clear:
    std::cout << "\x1B[2J\x1B[H";

    const std::size_t W = mapWidth();
    const std::size_t H = mapHeight();
    if (W == 0 || H == 0) return;

    // ANSI color codes
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[41m"; // Red background
    const std::string GREEN = "\033[42m"; // Green background for start
    const std::string BLUE = "\033[44m"; // Blue background for end
    const std::string YELLOW = "\033[43m"; // Yellow background for path
    const std::string WHITE_TEXT = "\033[97m"; // White text color
    const std::string WHITE_BG = "\033[47m"; // White background for obstacles

    // How many digits to print each column index (e.g. 0…99 → 2 digits)
    auto needed_digits = [&](std::size_t n){
        int d = 1;
        while (n >= 10) { n /= 10; ++d; }
        return d;
    };
    const int colWidth = needed_digits(W - 1);

    // How many digits for the row labels
    const int rowWidth = needed_digits(H - 1);

    // --- 1) Column header line ---
    // indent for row‐labels + one space
    std::cout << std::string(rowWidth + 1, ' ');
    for (std::size_t x = 0; x < W; ++x) {
        // each number is right‐aligned in a field of width colWidth
        std::cout << std::setw(colWidth) << x;
        // No space between column headers
    }
    std::cout << "\n";

    // --- 2) Each map row ---
    for (std::size_t y = 0; y < H; ++y) {
        // row label right‐aligned in rowWidth, plus a space
        std::cout << std::setw(rowWidth) << y << ' ';

        for (std::size_t x = 0; x < W; ++x) {
            GridCoordinate pos{ static_cast<int>(x), static_cast<int>(y) };
            char c = '#';
            std::string color = RED; // Default for walls is red background

            // draw background
            if (x < map[y].size()) {
                if (auto [type, cost] = map[y][x]; type == TileType::trail) {
                    c = hideCosts
                        ? ' '
                        : static_cast<char>('0' + cost);
                    color = ""; // No background color for normal trail
                }
            }

            if (!path.empty()) {
                if (pos == path.front()) {
                    c = 'S';
                    color = GREEN; // Start point in green
                } else if (pos == path.back()) {
                    c = 'E';
                    color = BLUE; // End point in blue
                } else if (std::ranges::find(path, pos) != path.end()) {
                    c = 'o';
                    color = YELLOW; // Path points in yellow
                }
            }

            if (c == '#') {
                color = WHITE_BG;
                c = ' ';
            }

            std::cout << color << WHITE_TEXT << std::string(colWidth, c) << RESET;
        }
        std::cout << "\n";
    }
}

CMapIterator& CGridtypeMap::leapIn(const GridCoordinate& pos)
{
    it.jumpToPosition(pos);
    return it;
}