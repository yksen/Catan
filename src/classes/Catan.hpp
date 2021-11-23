#ifndef Catan_H
#define Catan_H

#include "Map.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>

enum Stage
{
    menu,
    settings,
    game,
    leaderboard
};

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

    int citySize = borderSize * 10;

    int numberSize = bgTileSize / 4;
    int diceSize = 200;

    std::vector<std::mt19937::result_type> diceRolls = {0, 0};

    std::vector<sf::RectangleShape> buildings;

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

    void draw()
    {
        this->window->clear();
        switch (this->stage)
        {
        case menu:
            drawMenu();
            break;
        case settings:
            drawSettings();
            break;
        case game:
            drawGame();
            break;
        case leaderboard:
            drawLeaderboard();
            break;
        }
        this->window->display();
    }
    void drawCenterCross()
    {
        auto size = this->windowSize;
        sf::RectangleShape xAxis(sf::Vector2f(size.x, 3));
        sf::RectangleShape yAxis(sf::Vector2f(3, size.y));
        xAxis.setFillColor(sf::Color::Red);
        yAxis.setFillColor(sf::Color::Red);
        xAxis.setPosition(sf::Vector2f(0, size.y / 2 - 1.5));
        yAxis.setPosition(sf::Vector2f(size.x / 2 - 1.5, 0));
        this->window->draw(xAxis);
        this->window->draw(yAxis);
    }
    void drawMenu()
    {
        auto text = centerText("Menu");
        this->window->draw(text);
        drawCenterCross();
    }
    void drawSettings()
    {
        auto text = centerText("Settings");
        this->window->draw(text);
        drawCenterCross();
    }
    void drawGame()
    {
        drawDice();
        drawMap();
    }
    void drawLeaderboard()
    {        
        auto text = centerText("Leaderboard");
        this->window->draw(text);
        drawCenterCross();
    }
    void drawMap()
    {
        drawTiles();
        drawNumbers();
        drawBuildings();
    }
    void drawTiles()
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

                    if (tile.type == ocean)
                        bgTileShape.setFillColor(sf::Color(26, 140, 255));
                    else
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
    void drawNumbers()
    {
        for (size_t y = 0; y < map.height; y++)
        {
            for (size_t x = 0; x < map.width; x++)
            {
                Tile tile = map.tileMap[x][y];
                if (tile.type != none && tile.type != ocean && tile.type != desert)
                {
                    sf::Vector2f position(bgTileWidth / 2 + x * bgTileWidth + ((y + 1) % 2) * (bgTileWidth / 2), bgTileHeight / 2 + y * (3 * (bgTileHeight / 4)));
                    sf::CircleShape numberShape(numberSize);
                    sf::Text numberText;

                    numberShape.setOrigin(numberSize, numberSize);
                    numberShape.setPosition(position);

                    int diceSum = this->diceRolls[0] + this->diceRolls[1];
                    (tile.number == diceSum) ? numberShape.setFillColor(sf::Color::Green) : numberShape.setFillColor(sf::Color::White);

                    numberText.setFont(this->font);
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
    void drawBuildings()
    {
        for (auto building : this->buildings)
            this->window->draw(building);
    }
    void drawDice()
    {
        if (this->diceRolls[0] != 0)
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
    }

    void nextStage()
    {
        this->stage = Stage((this->stage + 1) % 4);
    }
    void setMap(std::string mapName)
    {
        map.loadFromFile(mapName);
    }
    void rollTheDice()
    {
        std::uniform_int_distribution<std::mt19937::result_type> diceDist(1, 6);
        std::vector<unsigned long> diceRolls = {diceDist(rng), diceDist(rng)};
        this->diceRolls = diceRolls;
    }
    void placeBuilding(sf::Vector2f mousePos)
    {
        sf::RectangleShape building(sf::Vector2f(this->citySize, this->citySize));

        building.setOrigin(sf::Vector2f(this->citySize / 2, this->citySize / 2));
        building.setPosition(mousePos);

        building.setTexture(&buildingsTextures);
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);
        building.setTextureRect(sf::IntRect(dist(rng) * this->textureSize, 0, this->textureSize, this->textureSize));

        this->buildings.push_back(building);
    }

    sf::Text centerText(std::string txt)
    {
        auto size = this->windowSize;
        sf::Text text(txt, font);

        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(size.y / 20);

        sf::FloatRect textSize = text.getLocalBounds();
        text.setOrigin(textSize.left + textSize.width / 2, textSize.top + textSize.height / 2);
        text.setPosition(sf::Vector2f(size.x / 2, size.y / 2));

        return text;
    }
    
};

#endif