#ifndef Building_H
#define Building_H

#include "Player.hpp"

class Building
{
public:
    sf::Vector2i position;
    Player* owner;
    bool isBuilt = false;
    sf::RectangleShape rect;
    sf::CircleShape circle;
    int level = 1;

    Building()
    {
    }
};

#endif