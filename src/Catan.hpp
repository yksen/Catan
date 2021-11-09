#ifndef Catan_H
#define Catan_H
#include "Map.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <string>

enum Stage { menu, settings, game };
std::vector<sf::Color> TileColors = { sf::Color::Blue, sf::Color(252, 84, 0), sf::Color(28, 120, 28), sf::Color(166, 166, 166), sf::Color(255, 221, 0), sf::Color::Green };

class Catan {
    private:
        sf::RenderWindow* window;
        Stage stage;
        Map map;  

        int tileSize = 90;
        int tileWidth = tileSize * sqrt(3);
        int tileHeight =  tileSize * 2;

    public:        
        Catan(sf::RenderWindow* x) {
            window = x;
            stage = menu;
        }
        
        Stage getStage() { return stage; }
        void setMap(std::string mapName) { map.setMap(mapName); }

        void drawGame()
        {
            window->clear();
            drawMap();
            window->display();
        }  

        void drawMap()
        {
            for (size_t y = 0; y < map.height; y++)
            {
                for (size_t x = 0; x < map.width; x++)
                {
                    sf::CircleShape tile(tileSize, 6);
                    sf::Vector2f position(x * tileWidth + ((y + 1) % 2) * (tileWidth / 2), y * (3 * (tileHeight / 4)));
                    
                    tile.setPosition(position);
                    tile.setFillColor(TileColors[map.tileMap[x][y].type]);
                
                    window->draw(tile);
                }
            }
        }

        void rollTheDice()
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist(1, 6);

            std::vector<unsigned long> diceRolls = {dist(rng), dist(rng)};

            std::cout << diceRolls[0] << diceRolls[1] << std::endl;
        }
};

#endif