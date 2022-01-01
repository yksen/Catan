#ifndef Tile_H
#define Tile_H

#include "Building.hpp"
#include <vector>
#include <SFML/System.hpp>

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
    sf::Vector2i position;
    TileType type;
    int number;
    std::vector<City> vertices;
    std::vector<Road> edges;

    Tile(int a = 0, int b = 0, TileType c = none, int d = 0)
        : position(a, b),
          type(c),
          number(d)
    {
        this->vertices.resize(6);
        this->edges.resize(6);
    }

    bool isLand()
    {
        if (type != none && type != ocean)
            return true;
        else
            return false;
    }
};

#endif