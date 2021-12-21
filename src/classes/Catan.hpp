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
    sf::Texture tileTextures, buildingsTextures, diceTextures, resourceTextures;

    const int textureSize = 256;
    const int resourceTextureSize = 128;
    const int resourceTextureWidth = 171;
    const int resourceTextureX = 22;
    const int resourceTextureY = 64;

    float gameStateWidth;
    float gameStateHeight;
    float gSW;
    float gSH;

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

    std::vector<Player> players = {Player(L"abc", sf::Color::Green), Player(L"xyz", sf::Color::Yellow), Player(L"gżegżółka", sf::Color::Red), Player(L"test123", sf::Color::Blue)};
    std::vector<Player>::iterator currentPlayer = players.begin();

    Product chosenProduct;

    bool debugingEnabled = false;

    Catan(sf::RenderWindow *a, sf::Vector2u b)
        : window(a),
          windowSize(b),
          gameStateWidth(windowSize.x / 4),
          gameStateHeight(windowSize.y),
          gSW(gameStateWidth / 100),
          gSH(gameStateHeight / 100)
    {
        this->tileTextures.loadFromFile("../assets/textures/tiles.png");
        this->tileTextures.setSmooth(true);
        this->buildingsTextures.loadFromFile("../assets/textures/buildings.png");
        this->buildingsTextures.setSmooth(true);
        this->diceTextures.loadFromFile("../assets/textures/dice_faces.png");
        this->diceTextures.setSmooth(true);
        this->resourceTextures.loadFromFile("../assets/textures/resources.png");
        this->resourceTextures.setSmooth(true);
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
            window->clear(sf::Color(0, 157, 174));
            drawGame();
            break;
        case leaderboard:
            drawLeaderboard();
            break;
        }
        if (debugingEnabled)
            debug();
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
        std::vector<sf::Color> colorPalette = {
            sf::Color(0, 157, 174),
            sf::Color(113, 223, 231),
            sf::Color(194, 255, 249),
            sf::Color(255, 230, 82)};

        sf::RectangleShape background(sf::Vector2f(gameStateWidth, gameStateHeight));
        background.setFillColor(colorPalette[1]);
        background.setOutlineColor(sf::Color::White);
        background.setOutlineThickness(gSW);
        sf::Vector2f backgroundPos(windowSize.x - gameStateWidth, 0);
        sf::Vector2f backgroundCenterOffset((int)-windowSize.x + gameStateWidth / 2, -gameStateHeight / 2);
        background.setPosition(backgroundPos);
        window->draw(background);

        sf::RectangleShape diceBackground(sf::Vector2f(90 * gSW, 20 * gSH));
        diceBackground.setFillColor(colorPalette[2]);
        diceBackground.setOrigin(backgroundCenterOffset);
        diceBackground.setPosition(-45 * gSW, -48 * gSH);
        window->draw(diceBackground);

        if (diceRolls[0] != 0)
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

        sf::Vector2f playerBackgroundSize(90 * gSW, 13 * gSH);
        sf::RectangleShape playerBackground(playerBackgroundSize);
        playerBackground.setFillColor(colorPalette[3]);
        playerBackground.setOrigin(backgroundCenterOffset);

        for (size_t i = 0; i < players.size(); i++)
        {
            auto player = players[i];
            playerBackground.setPosition(sf::Vector2f(-45 * gSW, -26 * gSH + i * (playerBackgroundSize.y + 2 * gSH)));
            sf::Vector2f playerBackgroundOffset((int)-windowSize.x + gameStateWidth - 5 * gSW, -gameStateHeight / 2 + 26 * gSH - i * (playerBackgroundSize.y + 2 * gSH));
            sf::Text playerName(player.name, font);
            playerName.setCharacterSize(3 * gSH);
            playerName.setFillColor(player.color);
            playerName.setOutlineThickness(1);
            playerName.setOrigin(playerBackgroundOffset);
            playerName.setPosition(sf::Vector2f(2 * gSW, 0));

            sf::Text playerPoints(std::to_string(player.points), font);
            playerPoints.setCharacterSize(3 * gSH);
            playerPoints.setFillColor(sf::Color::White);
            playerPoints.setOutlineThickness(1);
            playerPoints.setOrigin(playerBackgroundOffset);
            playerPoints.setPosition(sf::Vector2f(88 * gSW - playerPoints.getLocalBounds().width, 0));

            window->draw(playerBackground);
            window->draw(playerName);
            window->draw(playerPoints);

            for (size_t j = 0; j < player.buildings.size(); j++)
            {
                sf::RectangleShape buildingRect(sf::Vector2f(3 * gSH, 3 * gSH));
                buildingRect.setFillColor(player.color);
                buildingRect.setTexture(&buildingsTextures);
                buildingRect.setTextureRect(sf::IntRect(j * textureSize, 0, textureSize, textureSize));
                buildingRect.setOrigin(playerBackgroundOffset);
                buildingRect.setPosition(sf::Vector2f(2 * gSW + j * (3 * gSH + gSW), 9 * gSH));
                window->draw(buildingRect);

                sf::Text amountText(std::to_string(player.buildings.at(Product(j))), font);
                amountText.setCharacterSize(3 * gSH);
                amountText.setOutlineThickness(1);
                amountText.setOrigin(playerBackgroundOffset);
                auto textRect = amountText.getLocalBounds();
                amountText.setPosition(sf::Vector2f(2 * gSW + j * (3 * gSH + gSW) + 1.5 * gSH - textRect.width / 2, 5 * gSH));
                window->draw(amountText);
            }

            for (size_t j = 0; j < player.resources.size(); j++)
            {
                sf::RectangleShape resourceRect(sf::Vector2f(3 * gSH, 3 * gSH));
                resourceRect.setTexture(&resourceTextures);
                resourceRect.setTextureRect(sf::IntRect(j * resourceTextureWidth + resourceTextureX, resourceTextureY, resourceTextureSize, resourceTextureSize));
                resourceRect.setOrigin(playerBackgroundOffset);
                resourceRect.setPosition(sf::Vector2f(6 * gSW + (3 + j) * (3 * gSH + gSW), 9 * gSH));
                window->draw(resourceRect);

                if (currentPlayer == players.begin() + i)
                {
                    sf::Text amountText(std::to_string(player.resources.at(Resource(j))), font);
                    amountText.setCharacterSize(3 * gSH);
                    amountText.setOutlineThickness(1);
                    amountText.setOrigin(playerBackgroundOffset);
                    auto textRect = amountText.getLocalBounds();
                    amountText.setPosition(sf::Vector2f(6 * gSW + (3 + j) * (3 * gSH + gSW) + 1.5 * gSH - textRect.width / 2, 5 * gSH));
                    window->draw(amountText);
                }
            }

            sf::Vector2f backgroundBottomLeftOffset((int)-windowSize.x + gameStateWidth, -gameStateHeight);
            for (size_t j = 0; j < 3; j++)
            {
                sf::Vector2f buttonSize(30 * gSW, 30 * gSW);
                sf::RectangleShape button(buttonSize);
                button.setFillColor(colorPalette[2]);
                button.setOrigin(backgroundCenterOffset);
                button.setPosition(sf::Vector2f());

                window->draw(button);
            }
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
                        bgTileShape.setFillColor(sf::Color(0, 157, 174));
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
                            if (building.isBuilt && building.level == 1 && building.owner.id == currentPlayer->id)
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
        if (type != card && currentPlayer->buildings[type] <= 0)
            return false;

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
        currentPlayer->buildings[chosenProduct] -= 1;
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
                        if (spotCircle.getGlobalBounds().contains(mousePos) && spot.level == 1 && spot.owner.id == currentPlayer->id)
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
    void processMouseClick(sf::Vector2f mousePos)
    {
        if (turnState == build)
            placeBuilding(chosenProduct, mousePos);
    }

    void debug()
    {
        sf::Text text("", font);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
        text.setStyle(sf::Text::Bold);

        sf::Vector2f pos(0, 0);
        text.setString("Map name: " + map.name);
        window->draw(text);

        pos.y += text.getCharacterSize();
        text.setPosition(pos);
        text.setString("Game State: " + std::to_string(gameState));
        window->draw(text);

        pos.y += text.getCharacterSize();
        text.setPosition(pos);
        text.setString("Turn State: " + std::to_string(turnState));
        window->draw(text);
    }
};

#endif