#ifndef Player_H
#define Player_H

#include <string>
#include <SFML/Graphics.hpp>

enum Resource
{
    brick,
    lumber,
    ore,
    grain,
    wool
};

class Player
{
public:
    std::string name;
    sf::Color color;
    int points = 0;
    std::map<Resource, int> resources{
        {brick, 999}, {lumber, 999}, {ore, 999}, {grain, 999}, {wool, 999}
    };

    Player()
    {
    }
    Player(std::string name, sf::Color color)
    {
        this->name = name;
        this->color = color;
    }
};

#endif