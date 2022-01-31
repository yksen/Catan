#ifndef Tile_H
#define Tile_H

#include "Building.hpp"
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
    sf::Vector2u position;
    sf::Vector2f windowPosition;
    TileType type;
    int number;

    Tile(int a = 0, int b = 0, TileType c = none, int d = 0)
        : position(a, b),
          type(c),
          number(d)
    {
    }

    bool isLand()
    {
        if (type == none || type == ocean)
            return false;
        else
            return true;
    }
};

#endif