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
    std::vector<City> vertices;
    std::vector<Road> edges;

    Tile(int a = 0, int b = 0, TileType c = none, int d = 0)
    {
        this->x = a;
        this->y = b;
        this->type = c;
        this->number = d;
        this->vertices.resize(6);
        this->edges.resize(6);
    }

    bool isLand()
    {
        if (this->type != none && this->type != ocean)
            return true;
        else
            return false;
    }
};

#endif