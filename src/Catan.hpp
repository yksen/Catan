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
std::vector<sf::Color> TileColors = { sf::Color::Blue, sf::Color(252, 84, 0), sf::Color(28, 120, 28), sf::Color(166, 166, 166), sf::Color(255, 221, 0), sf::Color::Green, sf::Color(255, 247, 135) };

std::uniform_int_distribution<std::mt19937::result_type> diceDist(1, 6);

class Catan {
    private:
        sf::RenderWindow* window;
        Stage stage;
        Map map;  

        sf::Texture textures;
        int textureSize = 256;

        int tileSize = 90;
        int tileWidth = tileSize * sqrt(3);
        int tileHeight =  tileSize * 2;

    public:        
        Catan(sf::RenderWindow* x) {
            window = x;
            textures.loadFromFile("../assets/textures.png");
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
                    auto type = map.tileMap[x][y].type;

                    tile.setPosition(position);
                    // tile.setFillColor(TileColors[type]);
                    tile.setTexture(&textures);
                    tile.setTextureRect(sf::IntRect((type % 4) * textureSize, ((type / 4) % 4) * textureSize, textureSize, textureSize));
                
                    window->draw(tile);
                }
            }
        }

        void rollTheDice()
        {
            std::vector<unsigned long> diceRolls = {diceDist(rng), diceDist(rng)};

            std::cout << diceRolls[0] << diceRolls[1] << std::endl;
        }
};

#endif