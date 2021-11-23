#ifndef Player_H
#define Player_H

#include <string>
#include <SFML/Graphics.hpp>

class Player
{
public:
    std::string name;
    sf::Color color;
    int points = 0;

    Player()
    {
    }
};

#endif