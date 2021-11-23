#ifndef Catan_H
#define Catan_H

#include "Map.hpp"
#include "Player.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>

enum Stage
{
    menu,
    settings,
    game
};
std::vector<sf::Color> TileColors = {sf::Color::Blue, sf::Color(252, 84, 0), sf::Color(28, 120, 28), sf::Color(166, 166, 166), sf::Color(255, 221, 0), sf::Color::Green, sf::Color(255, 247, 135)};

class Catan
{
public:
    sf::RenderWindow *window;
    sf::Vector2u windowSize;

    Stage stage;
    Map map;

    sf::Font font;
    sf::Texture tileTextures, buildingsTextures, diceTextures;
    const int textureSize = 256;

    int bgTileSize = 95;
    int bgTileWidth = bgTileSize * sqrt(3);
    int bgTileHeight = bgTileSize * 2;

    int borderSize = bgTileSize / 19;
    int tileSize = bgTileSize - borderSize;

    int numberSize = bgTileSize / 4;
    int diceSize = 200;

    std::vector<unsigned long> diceRolls;

    Catan(sf::RenderWindow *a, sf::Vector2u b)
    {
        this->window = a;
        this->windowSize = b;
        this->tileTextures.loadFromFile("../assets/textures/tiles.png");
        this->buildingsTextures.loadFromFile("../assets/textures/buildings.png");
        this->diceTextures.loadFromFile("../assets/textures/dice_faces.png");
        this->tileTextures.setSmooth(true);
        this->buildingsTextures.setSmooth(true);
        this->diceTextures.setSmooth(true);
        this->font.loadFromFile("../assets/fonts/arial.ttf");
        this->stage = menu;
    }

    void setMap(std::string mapName) { map.loadFromFile(mapName); }

    void drawGame()
    {
        this->window->clear();
        drawDice();
        drawMap();
        this->window->display();
    }
    void drawDice()
    {
        for (size_t i = 0; i < this->diceRolls.size(); i++)
        {
            auto value = this->diceRolls[i] - 1;

            sf::RectangleShape diceFace(sf::Vector2f(this->diceSize, this->diceSize));

            diceFace.setPosition(this->windowSize.x - this->diceSize, i * this->diceSize);

            diceFace.setTexture(&this->diceTextures);
            diceFace.setTextureRect(sf::IntRect((value % 3) * this->textureSize, ((value / 3) % 3) * this->textureSize, this->textureSize, this->textureSize));

            this->window->draw(diceFace);
        }
    }
    void drawMap()
    {
        for (size_t y = 0; y < map.height; y++)
        {
            for (size_t x = 0; x < map.width; x++)
            {
                Tile tile = map.tileMap[x][y];
                if (tile.type != none)
                {
                    sf::CircleShape bgTileShape(bgTileSize, 6);
                    sf::CircleShape tileShape(tileSize, 6);

                    bgTileShape.setOrigin(bgTileSize, bgTileSize);
                    tileShape.setOrigin(tileSize, tileSize);

                    sf::Vector2f position(bgTileWidth / 2 + x * bgTileWidth + ((y + 1) % 2) * (bgTileWidth / 2), bgTileHeight / 2 + y * (3 * (bgTileHeight / 4)));
                    bgTileShape.setPosition(position);
                    tileShape.setPosition(position);

                    // bgTile.setFillColor(TileColors[type]);
                    bgTileShape.setFillColor(sf::Color::White);
                    tileShape.setTexture(&tileTextures);
                    tileShape.setTextureRect(sf::IntRect(((tile.type - 1) % 4) * textureSize, (((tile.type - 1) / 4) % 4) * textureSize, textureSize, textureSize));

                    this->window->draw(bgTileShape);
                    this->window->draw(tileShape);

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

                        this->window->draw(numberShape);
                        this->window->draw(numberText);
                    }
                }
            }
        }
    }

    void rollTheDice()
    {
        std::uniform_int_distribution<std::mt19937::result_type> diceDist(1, 6);
        std::vector<unsigned long> diceRolls = {diceDist(rng), diceDist(rng)};
        this->diceRolls = diceRolls;
    }
};

#endif