#ifndef Building_H
#define Building_H

#include "Player.hpp"

class Building
{
public:
    Player owner;
    bool isBuilt = false;
    sf::RectangleShape rect;
    sf::CircleShape spot;
};

class City : public Building
{
public:
    int level = 1;
    City()
    {
    }
};

class Road : public Building
{
public:
    Road()
    {
    }
};

#endif