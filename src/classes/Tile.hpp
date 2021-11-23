#ifndef Tile_H
#define Tile_H

#include "Building.hpp"
#include <vector>

enum TileType
{
    none,
    ocean,
    hill,
    forest,
    mountain,
    field,
    pasture,
    desert
};

class Tile
{
public:
    int x, y;
    TileType type;
    int number;
    std::vector<Building> edges;

    Tile(int a = 0, int b = 0, TileType c = none, int d = 0)
    {
        this->x = a;
        this->y = b;
        this->type = c;
        this->number = d;
    }
};

#endif