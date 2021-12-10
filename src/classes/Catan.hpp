#ifndef Catan_H
#define Catan_H

#include "Map.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>

enum GameState
{
    menu,
    settings,
    game,
    leaderboard
};
enum TurnState
{
    setup,
    dice,
    idle,
    build,
    trade
};
std::map<TileType, Resource> TileProduct{
    {hill, brick},
    {forest, lumber},
    {mountain, ore},
    {field, grain},
    {pasture, wool}};
enum Product
{
    settlement,
    city,
    road,
    card
};
std::map<Product, std::map<Resource, int>> ProductCost{
    {settlement, {{brick, 1}, {lumber, 1}, {grain, 1}, {wool, 1}}},
    {city, {{ore, 3}, {grain, 2}}},
    {road, {{brick, 1}, {lumber, 1}}},
    {card, {{ore, 1}, {grain, 1}, {wool, 1}}}};

class Catan
{
public:
    sf::RenderWindow *window;
    sf::Vector2u windowSize;

    GameState gameState = menu;
    TurnState turnState = setup;
    Map map;

    sf::Font font;
    sf::Texture tileTextures, buildingsTextures, diceTextures;

    const int textureSize = 256;

    float gameStateWidth;
    float gameStateHeight;

    float bgTileSize = 95;
    float bgTileWidth = bgTileSize * sqrt(3);
    float bgTileHeight = bgTileSize * 2;

    float borderSize = bgTileSize / 19;
    float tileSize = bgTileSize - borderSize;

    float buildingSize = borderSize * 10;
    float spotSize = buildingSize / 4;

    float numberSize = bgTileSize / 4;

    sf::Color spotColor = sf::Color::Cyan;

    std::vector<std::mt19937::result_type> diceRolls = {0, 0};

    std::vector<Player> players = {Player("abc", sf::Color::Yellow), Player("xyz", sf::Color::Red)};
    std::vector<Player>::iterator currentPlayer = players.begin();

    Product chosenProduct;

    Catan(sf::RenderWindow *a, sf::Vector2u b)
    {
        this->window = a;
        this->windowSize = b;
        this->gameStateWidth = windowSize.x / 4;
        this->gameStateHeight = windowSize.y;
        this->tileTextures.loadFromFile("../assets/textures/tiles.png");
        this->buildingsTextures.loadFromFile("../assets/textures/buildings.png");
        this->diceTextures.loadFromFile("../assets/textures/dice_faces.png");
        this->tileTextures.setSmooth(true);
        this->buildingsTextures.setSmooth(true);
        this->diceTextures.setSmooth(true);
        this->font.loadFromFile("../assets/fonts/arial.ttf");
    }

    void draw()
    {
        window->clear();
        switch (gameState)
        {
        case menu:
            drawMenu();
            break;
        case settings:
            drawSettings();
            break;
        case game:
            window->clear(sf::Color(30, 30, 200));
            drawGame();
            break;
        case leaderboard:
            drawLeaderboard();
            break;
        }
        window->display();
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
        drawMap();
        drawGameState();
    }
    void drawLeaderboard()
    {
        auto text = centerText("Leaderboard");
        this->window->draw(text);
        drawCenterCross();
    }
    void drawGameState()
    {
        float gSW = gameStateWidth / 100;
        float gSH = gameStateHeight / 100;

        sf::RectangleShape background(sf::Vector2f(gameStateWidth, gameStateHeight));
        background.setFillColor(sf::Color::White);
        background.setOutlineColor(sf::Color::Black);
        background.setOutlineThickness(gSW);
        sf::Vector2f backgroundPos(windowSize.x - gameStateWidth, 0);
        sf::Vector2f backgroundCenterOffset((int)-windowSize.x + gameStateWidth / 2, -gameStateHeight / 2);
        background.setPosition(backgroundPos);
        window->draw(background);

        sf::RectangleShape diceBackground(sf::Vector2f(90 * gSW, 20 * gSH));
        diceBackground.setFillColor(sf::Color::Cyan);
        diceBackground.setOutlineColor(sf::Color::Black);
        diceBackground.setOutlineThickness(gSW);
        diceBackground.setOrigin(backgroundCenterOffset);
        diceBackground.setPosition(-45 * gSW, -48 * gSH);
        window->draw(diceBackground);

        if (this->diceRolls[0] != 0)
            for (size_t i = 0; i < diceRolls.size(); i++)
            {
                auto value = diceRolls[i] - 1;
                sf::RectangleShape diceFace(sf::Vector2f(18 * gSH, 18 * gSH));
                diceFace.setOrigin(backgroundCenterOffset.x, backgroundCenterOffset.y + 48 * gSH);
                diceFace.setPosition(-42 * gSW + i * 43.5 * gSW, 1 * gSH);
                diceFace.setTexture(&diceTextures);
                diceFace.setTextureRect(sf::IntRect((value % 3) * textureSize, ((value / 3) % 3) * textureSize, textureSize, textureSize));
                window->draw(diceFace);
            }

        

    }
    void drawMap()
    {
        drawTiles();
        drawNumbers();
        drawBuildings();
        drawBuildingSpots(chosenProduct);
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
        for (auto tileRow : map.tileMap)
        {
            for (auto tile : tileRow)
            {
                for (auto building : tile.edges)
                    if (building.isBuilt)
                        window->draw(building.rect);
                for (auto building : tile.vertices)
                    if (building.isBuilt)
                        window->draw(building.rect);
            }
        }
    }
    void drawBuildingSpots(Product type)
    {
        if (turnState == build)
        {
            for (auto tileRow : map.tileMap)
            {
                for (auto tile : tileRow)
                {
                    if (type == settlement)
                    {
                        for (auto building : tile.vertices)
                        {
                            if (!building.isBuilt)
                                this->window->draw(building.circle);
                        }
                    }
                    if (type == city)
                    {
                        for (auto building : tile.vertices)
                        {
                            if (building.isBuilt && building.level == 1)
                                this->window->draw(building.circle);
                        }
                    }
                    if (type == road)
                    {
                        for (auto building : tile.edges)
                        {
                            if (!building.isBuilt)
                                this->window->draw(building.circle);
                        }
                    }
                }
            }
        }
    }

    void endTurn()
    {
        turnState = (turnState == setup) ? setup : dice;
        currentPlayer = (currentPlayer + 1 < players.end()) ? currentPlayer + 1 : players.begin();
    }
    bool canBuy(Product type)
    {
        auto cost = ProductCost[type];
        for (auto pair : cost)
        {
            if (currentPlayer->resources[pair.first] < pair.second)
                return false;
        }
        return true;
    }
    void chooseProduct(Product type)
    {
        chosenProduct = type;
        if (canBuy(chosenProduct))
            turnState = build;
    }
    void payForProduct(Product type)
    {
        auto cost = ProductCost[type];
        for (auto pair : cost)
        {
            currentPlayer->resources[pair.first] -= pair.second;
        }
    }
    sf::Vector2f getVertexShift(int n)
    {
        switch (n)
        {
        case 0:
            return sf::Vector2f(0, -bgTileHeight / 2);
            break;
        case 1:
            return sf::Vector2f(bgTileWidth / 2, -bgTileHeight / 4);
            break;
        case 2:
            return sf::Vector2f(bgTileWidth / 2, bgTileHeight / 4);
            break;
        case 3:
            return sf::Vector2f(0, bgTileHeight / 2);
            break;
        case 4:
            return sf::Vector2f(-bgTileWidth / 2, bgTileHeight / 4);
            break;
        case 5:
            return sf::Vector2f(-bgTileWidth / 2, -bgTileHeight / 4);
            break;
        }
        return sf::Vector2f(0, 0);
    }
    sf::Vector2f getEdgeShift(int n)
    {
        switch (n)
        {
        case 0:
            return sf::Vector2f(bgTileWidth / 4, -1.5 * bgTileHeight / 4);
            break;
        case 1:
            return sf::Vector2f(bgTileWidth / 2, 0);
            break;
        case 2:
            return sf::Vector2f(bgTileWidth / 4, 1.5 * bgTileHeight / 4);
            break;
        case 3:
            return sf::Vector2f(-bgTileWidth / 4, 1.5 * bgTileHeight / 4);
            break;
        case 4:
            return sf::Vector2f(-bgTileWidth / 2, 0);
            break;
        case 5:
            return sf::Vector2f(-bgTileWidth / 4, -1.5 * bgTileHeight / 4);
            break;
        }
        return sf::Vector2f(0, 0);
    }
    void nextGameState()
    {
        this->gameState = GameState((this->gameState + 1) % 4);
    }
    void setMap(std::string mapName)
    {
        map.loadFromFile(mapName);
        generateBuildingSpots();
    }
    void rollTheDice()
    {
        if (turnState == dice)
        {
            std::uniform_int_distribution<std::mt19937::result_type> diceDist(1, 6);
            std::vector<std::mt19937::result_type> diceRolls = {diceDist(rng), diceDist(rng)};
            this->diceRolls = diceRolls;
        }
    }
    void placeBuilding(Product type, sf::Vector2f mousePos)
    {
        if (turnState == build)
        {
            for (auto &tileRow : map.tileMap)
            {
                for (auto &tile : tileRow)
                {
                    if (type == settlement)
                    {
                        for (auto &spot : tile.vertices)
                        {
                            auto spotCircle = spot.circle;
                            if (spotCircle.getGlobalBounds().contains(mousePos))
                            {
                                sf::RectangleShape building(sf::Vector2f(this->buildingSize, this->buildingSize));

                                building.setOrigin(sf::Vector2f(this->buildingSize / 2, this->buildingSize / 2));
                                building.setPosition(spotCircle.getGlobalBounds().left + spotCircle.getGlobalBounds().width / 2, spotCircle.getGlobalBounds().top + spotCircle.getGlobalBounds().height / 2);
                                building.setTexture(&buildingsTextures);
                                building.setTextureRect(sf::IntRect(0, 0, this->textureSize, this->textureSize));
                                building.setFillColor(currentPlayer->color);
                                spot.rect = building;

                                payForProduct(chosenProduct);
                                spot.isBuilt = true;
                                spot.owner = *currentPlayer;
                                currentPlayer->points += 1;
                                turnState = idle;
                            }
                        }
                    }
                    if (type == city)
                    {
                        for (auto &spot : tile.vertices)
                        {
                            auto spotCircle = spot.circle;
                            if (spotCircle.getGlobalBounds().contains(mousePos) && spot.level == 1)
                            {
                                auto &building = spot.rect;
                                building.setTextureRect(sf::IntRect(textureSize, 0, textureSize, textureSize));

                                payForProduct(chosenProduct);
                                spot.level = 2;
                                currentPlayer->points += 1;
                                turnState = idle;
                            }
                        }
                    }
                    if (type == road)
                    {
                        int i = 0;
                        for (auto &spot : tile.edges)
                        {
                            auto spotCircle = spot.circle;
                            if (spotCircle.getGlobalBounds().contains(mousePos))
                            {
                                sf::RectangleShape building(sf::Vector2f(this->buildingSize, this->buildingSize / 4));

                                building.setOrigin(sf::Vector2f(this->buildingSize / 2, this->buildingSize / 4 / 2));
                                building.setPosition(spotCircle.getGlobalBounds().left + spotCircle.getGlobalBounds().width / 2, spotCircle.getGlobalBounds().top + spotCircle.getGlobalBounds().height / 2);
                                building.setTexture(&buildingsTextures);
                                building.setTextureRect(sf::IntRect(this->textureSize * 2, 3 * this->textureSize / 4, this->textureSize, this->textureSize / 4));
                                building.setFillColor(currentPlayer->color);
                                building.setRotation(30 + 60 * (i % 3));
                                spot.rect = building;

                                payForProduct(chosenProduct);
                                spot.isBuilt = true;
                                spot.owner = *currentPlayer;
                                turnState = idle;
                            }
                            i++;
                        }
                    }
                }
            }
        }
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
    void generateBuildingSpots()
    {
        for (auto &tileRow : map.tileMap)
        {
            for (auto &tile : tileRow)
            {
                if (tile.isLand())
                {
                    for (size_t i = 0; i < 6; i++)
                    {
                        sf::CircleShape *spot = &tile.vertices[i].circle;
                        spot->setRadius(spotSize);
                        spot->setFillColor(spotColor);
                        spot->setOutlineColor(sf::Color::Black);
                        spot->setOutlineThickness(spotSize / 10);
                        spot->setOrigin(sf::Vector2f(spotSize, spotSize));
                        auto shift = getVertexShift(i);
                        sf::Vector2f position(
                            bgTileWidth / 2 + tile.x * bgTileWidth + ((tile.y + 1) % 2) * (bgTileWidth / 2) + shift.x,
                            bgTileHeight / 2 + tile.y * (3 * (bgTileHeight / 4)) + shift.y);
                        spot->setPosition(position);

                        spot = &tile.edges[i].circle;
                        spot->setRadius(spotSize);
                        spot->setFillColor(spotColor);
                        spot->setOutlineColor(sf::Color::Black);
                        spot->setOutlineThickness(spotSize / 10);
                        spot->setOrigin(sf::Vector2f(spotSize, spotSize));
                        shift = getEdgeShift(i);
                        position = sf::Vector2f(
                            bgTileWidth / 2 + tile.x * bgTileWidth + ((tile.y + 1) % 2) * (bgTileWidth / 2) + shift.x,
                            bgTileHeight / 2 + tile.y * (3 * (bgTileHeight / 4)) + shift.y);
                        spot->setPosition(position);
                    }
                }
            }
        }
    }

    void debug()
    {
        for (auto player : players)
            std::cout << player.name << ": " << player.points << std::endl;
    }
};

#endif