#ifndef Player_H
#define Player_H

#include <string>
#include <SFML/Graphics.hpp>

enum Product
{
    settlement,
    city,
    road,
    card
};
enum Resource
{
    brick,
    lumber,
    wool,
    grain,
    ore
};

int commonId = 0;

class Player
{
public:
    int id;
    std::wstring name;
    sf::Color color;
    int points = 0;
    std::map<Resource, int> resources{
        {brick, 9}, {lumber, 9}, {ore, 9}, {grain, 9}, {wool, 9}};
    std::map<Product, int> buildings{
        {settlement, 5}, {city, 4}, {road, 15}};

    Player()
    {
    }
    Player(std::wstring name, sf::Color color)
    {
        this->id = commonId++;
        this->name = name;
        this->color = color;
    }
};

#endif