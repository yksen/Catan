#ifndef Building_H
#define Building_H

#include "Player.hpp"

class Building
{
public:
    Player owner;
    bool isBuilt = false;
    sf::RectangleShape rect;
    sf::CircleShape circle;
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