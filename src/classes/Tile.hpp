#ifndef Tile_H
#define Tile_H

#include "Building.hpp"
#include <vector>

enum TileType { ocean, hill, forest, mountain, field, pasture, desert };

class Tile {
    private:
        int x, y;

    public:
        TileType type;
        int number;
        std::vector<Building> edges;

        Tile(int a = -1, int b = -1, TileType c = ocean, int d = 7) {
            x = a;
            y = b;
            type = c;
            number = d;
        }
};

#endif