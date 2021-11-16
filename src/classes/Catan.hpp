#ifndef Catan_H
#define Catan_H

#include "Map.hpp"
#include "Player.hpp"
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
        sf::Vector2u windowSize;

        sf::Font font;

        Stage stage;
        Map map;  

        sf::Texture tileTextures, diceTextures;
        int textureSize = 256;

        int bgTileSize = 95;
        int bgTileWidth = bgTileSize * sqrt(3);
        int bgTileHeight =  bgTileSize * 2;
        
        int borderSize = 5;    
        int tileSize = bgTileSize - borderSize;

        int numberSize = bgTileSize / 4;
        int diceSize = 200;

        std::vector<unsigned long> diceRolls;

    public:        
        Catan(sf::RenderWindow* a, sf::Vector2u b) {
            window = a;
            windowSize = b;
            tileTextures.loadFromFile("../assets/textures/textures.png");
            diceTextures.loadFromFile("../assets/textures/dice_faces.png");
            font.loadFromFile("../assets/fonts/arial.ttf");
            stage = menu;
        }
        
        void setMap(std::string mapName) { map.setMap(mapName); }

        void drawGame()
        {
            window->clear();
            drawDice();
            drawMap();
            window->display();
        }  
        void drawDice()
        {
            diceTextures.setSmooth(true);
            for (size_t i = 0; i < diceRolls.size(); i++)
            {
                auto value = diceRolls[i] - 1;
                sf::RectangleShape diceFace(sf::Vector2f(diceSize, diceSize));

                diceFace.setPosition(windowSize.x - diceSize, i * diceSize);

                diceFace.setTexture(&diceTextures);
                diceFace.setTextureRect(sf::IntRect((value % 3) * textureSize, ((value / 3) % 3) * textureSize, textureSize, textureSize));

                window->draw(diceFace);
            }
        }
        void drawMap()
        {
            tileTextures.setSmooth(true);
            for (size_t y = 0; y < map.height; y++)
            {
                for (size_t x = 0; x < map.width; x++)
                {
                    sf::CircleShape bgTileShape(bgTileSize, 6);
                    sf::CircleShape tileShape(tileSize, 6);
                    Tile tile = map.tileMap[x][y];

                    bgTileShape.setOrigin(bgTileSize, bgTileSize);
                    tileShape.setOrigin(tileSize, tileSize);

                    sf::Vector2f position(bgTileWidth / 2 + x * bgTileWidth + ((y + 1) % 2) * (bgTileWidth / 2), bgTileHeight / 2 + y * (3 * (bgTileHeight / 4)));
                    bgTileShape.setPosition(position);
                    tileShape.setPosition(position);
                    
                    // bgTile.setFillColor(TileColors[type]);
                    bgTileShape.setFillColor(sf::Color::White);
                    tileShape.setTexture(&tileTextures);
                    tileShape.setTextureRect(sf::IntRect((tile.type % 4) * textureSize, ((tile.type / 4) % 4) * textureSize, textureSize, textureSize));
                
                    window->draw(bgTileShape);
                    window->draw(tileShape);
                    
                    if (tile.type != ocean && tile.type != desert)
                    {
                        sf::CircleShape numberShape(numberSize);
                        sf::Text numberText;

                        numberShape.setOrigin(numberSize, numberSize);
                        numberShape.setPosition(position);
                        numberShape.setFillColor(sf::Color::White);
                        
                        numberText.setFont(font);
                        numberText.setFillColor(sf::Color::Black);
                        numberText.setStyle(sf::Text::Bold);
                        numberText.setString(std::to_string(tile.number));
                        numberText.setCharacterSize(std::floor(numberSize));

                        sf::FloatRect textSize = numberText.getLocalBounds();
                        numberText.setOrigin(textSize.width / 2, textSize.height);
                        numberText.setPosition(position);

                        window->draw(numberShape);
                        window->draw(numberText);
                    }
                }
            }
        }

        void rollTheDice()
        {
            std::vector<unsigned long> diceRolls = {diceDist(rng), diceDist(rng)};
            this->diceRolls = diceRolls;
        }
};

#endif