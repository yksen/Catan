#ifndef Building_H
#define Building_H

#include "Player.hpp"

class Building
{
public:
    Player owner;
};

class City : Building
{
public:
    int level = 1;
    City()
    {
    }
};

class Road : Building
{
public:
    Road()
    {
    }
};

#endif