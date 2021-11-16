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

        sf::Texture tileTextures, diceTextures;
        int textureSize = 256;

        int bgTileSize = 90;
        int bgTileWidth = bgTileSize * sqrt(3);
        int bgTileHeight =  bgTileSize * 2;
        
        int borderSize = 5;    
        int tileSize = bgTileSize - borderSize;

    public:        
        Catan(sf::RenderWindow* x) {
            window = x;
            tileTextures.loadFromFile("../assets/textures.png");
            diceTextures.loadFromFile("../assets/dice_faces.png");
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
                    sf::CircleShape bgTile(bgTileSize, 6);
                    sf::CircleShape tile(tileSize, 6);
                    auto type = map.tileMap[x][y].type;

                    bgTile.setOrigin(bgTileSize, bgTileSize);
                    tile.setOrigin(tileSize, tileSize);

                    sf::Vector2f position(bgTileWidth + x * bgTileWidth + ((y + 1) % 2) * (bgTileWidth / 2), bgTileHeight + y * (3 * (bgTileHeight / 4)));
                    bgTile.setPosition(position);
                    tile.setPosition(position);
                    
                    // bgTile.setFillColor(TileColors[type]);
                    bgTile.setFillColor(sf::Color::White);
                    tile.setTexture(&tileTextures);
                    tile.setTextureRect(sf::IntRect((type % 4) * textureSize, ((type / 4) % 4) * textureSize, textureSize, textureSize));
                
                    window->draw(bgTile);
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