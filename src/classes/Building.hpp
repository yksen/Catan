#ifndef Building_H
#define Building_H

#include "Player.hpp"

class Building
{
public:
    sf::Vector2i position;
    sf::Vector2f windowPosition;
    Product type;
    Player* owner;
    sf::RectangleShape rect;
    sf::CircleShape circle;
    int level = 1;
    bool isBuilt = false;
    bool isLand;

    Building()
    {
    }
};

#endif