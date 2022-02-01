#ifndef Catan_H
#define Catan_H

#include "Map.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <cmath>

enum GameState
{
    menu,
    settings,
    game,
    leaderboard
};
enum TurnState
{
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
    {devCard, {{ore, 1}, {grain, 1}, {wool, 1}}}};

class Catan
{
public:
    sf::RenderWindow *window;

    GameState gameState = game;
    TurnState turnState = build;
    Map map;

    sf::Font font;
    sf::Texture tileTextures, buildingsTextures, diceTextures, resourceTextures, robberTexture, playTexture, devCardTexture;

    const int textureSize = 256;
    const int resourceTextureSize = 128;
    const int resourceTextureWidth = 171;
    const int resourceTextureX = 22;
    const int resourceTextureY = 64;

    float gameStateWidth;
    float gameStateHeight;
    float gSW;
    float gSH;
    sf::Vector2f playerBackgroundSize;
    sf::Vector2f backgroundCenterOffset;

    float bgTileSize = 95;
    float bgTileWidth = bgTileSize * sqrt(3);
    float bgTileHeight = bgTileSize * 2;

    float borderSize = bgTileSize / 19;
    float tileSize = bgTileSize - borderSize;

    float buildingSize = borderSize * 10;
    float spotSize = buildingSize / 4;

    float numberSize = bgTileSize / 4;

    sf::Color spotColor = sf::Color::Cyan;

    std::vector<std::mt19937::result_type> diceRolls = {4, 3};

    std::vector<Player> players = {Player(L"abc", sf::Color::Green), Player(L"xyz", sf::Color::Yellow), Player(L"gżegżółka", sf::Color::Red), Player(L"test123", sf::Color::Blue)};
    std::vector<Player>::iterator currentPlayer = players.begin();

    sf::Vector2u placedCityPos;
    sf::Vector2u robberPos;

    bool robberActive = false;
    bool robbingActive = false;

    Product chosenProduct;

    bool setupEnabled = true;
    bool setupSecondTurn = false;
    bool debugingEnabled = false;
    bool drawCoordinates = false;

    std::vector<Building *> availableSpots;

    std::vector<sf::RectangleShape> gameButtons;
    std::vector<sf::RectangleShape> diceRects;
    std::vector<sf::RectangleShape> playerBackgroundsRects;

    std::vector<sf::CircleShape> numberShapes;
    std::vector<Tile> numberTiles;
    std::vector<sf::Text> numberTexts;

    std::vector<Player *> playersToBeRobbed;

    std::vector<sf::Color> colorPalette = {
        sf::Color(0, 157, 174),
        sf::Color(113, 223, 231),
        sf::Color(194, 255, 249),
        sf::Color(255, 230, 82),
        sf::Color(255, 211, 182)};

    Catan(sf::RenderWindow *a)
        : window(a),
          gameStateWidth(window->getSize().x / 4),
          gameStateHeight(window->getSize().y),
          gSW(gameStateWidth / 100),
          gSH(gameStateHeight / 100),
          backgroundCenterOffset((int)-window->getSize().x + gameStateWidth / 2, -gameStateHeight / 2),
          playerBackgroundSize(90 * gSW, 13 * gSH)
    {
        this->tileTextures.loadFromFile("../assets/textures/tiles.png");
        this->tileTextures.setSmooth(true);
        this->buildingsTextures.loadFromFile("../assets/textures/buildings.png");
        this->buildingsTextures.setSmooth(true);
        this->diceTextures.loadFromFile("../assets/textures/dice_faces.png");
        this->diceTextures.setSmooth(true);
        this->resourceTextures.loadFromFile("../assets/textures/resources.png");
        this->resourceTextures.setSmooth(true);
        this->robberTexture.loadFromFile("../assets/textures/robber.png");
        this->robberTexture.setSmooth(true);
        this->playTexture.loadFromFile("../assets/textures/play.png");
        this->playTexture.setSmooth(true);
        this->devCardTexture.loadFromFile("../assets/textures/dev_card.png");
        this->devCardTexture.setSmooth(true);
        this->font.loadFromFile("../assets/fonts/arial.ttf");

        generateButtons();
        generateDice();
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
        auto size = window->getSize();
        sf::RectangleShape xAxis(sf::Vector2f(size.x, 3));
        sf::RectangleShape yAxis(sf::Vector2f(3, size.y));
        xAxis.setFillColor(sf::Color::Red);
        yAxis.setFillColor(sf::Color::Red);
        xAxis.setPosition(sf::Vector2f(0, size.y / 2 - 1.5));
        yAxis.setPosition(sf::Vector2f(size.x / 2 - 1.5, 0));
        window->draw(xAxis);
        window->draw(yAxis);
    }
    void drawMenu()
    {
        auto text = centerText("Menu");
        window->draw(text);
        drawCenterCross();
    }
    void drawSettings()
    {
        auto text = centerText("Settings");
        window->draw(text);
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
        window->draw(text);
        drawCenterCross();
    }
    void drawGameState()
    {
        sf::RectangleShape background(sf::Vector2f(gameStateWidth, gameStateHeight));
        background.setFillColor(colorPalette[1]);
        background.setOutlineColor(sf::Color::White);
        background.setOutlineThickness(gSW);
        sf::Vector2f backgroundPos(window->getSize().x - gameStateWidth, 0);
        background.setPosition(backgroundPos);
        window->draw(background);

        // Dice background
        sf::RectangleShape diceBackground(sf::Vector2f(90 * gSW, 20 * gSH));
        diceBackground.setFillColor(colorPalette[2]);
        diceBackground.setOrigin(backgroundCenterOffset);
        diceBackground.setPosition(-45 * gSW, -48 * gSH);
        window->draw(diceBackground);
        // Dice
        for (auto rect : diceRects)
            window->draw(rect);

        // Player
        for (size_t i = 0; i < players.size(); i++)
        {
            auto player = players[i];
            auto playerBackground = playerBackgroundsRects[i];
            playerBackground.setPosition(sf::Vector2f(-45 * gSW, -23 * gSH + i * (playerBackgroundSize.y + 2 * gSH)));
            sf::Vector2f playerBackgroundOffset((int)-window->getSize().x + gameStateWidth - 5 * gSW, -gameStateHeight / 2 + 23 * gSH - i * (playerBackgroundSize.y + 2 * gSH));
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

            if (player.id == currentPlayer->id)
                playerBackground.setFillColor(colorPalette[4]);
            else
                playerBackground.setFillColor(colorPalette[3]);

            if (robbingActive)
                for (auto player2 : playersToBeRobbed)
                    if (player.id == player2->id)
                        playerBackground.setFillColor(spotColor);

            window->draw(playerBackground);
            window->draw(playerName);
            window->draw(playerPoints);

            // Buildings
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
            // Resources
            for (size_t j = 0; j < player.resources.size(); j++)
            {
                sf::RectangleShape resourceRect(sf::Vector2f(3 * gSH, 3 * gSH));
                resourceRect.setTexture(&resourceTextures);
                resourceRect.setTextureRect(sf::IntRect(j * resourceTextureWidth + resourceTextureX, resourceTextureY, resourceTextureSize, resourceTextureSize));
                resourceRect.setOrigin(playerBackgroundOffset);
                resourceRect.setPosition(sf::Vector2f(6 * gSW + (3 + j) * (3 * gSH + gSW), 9 * gSH));
                window->draw(resourceRect);

                // if (currentPlayer == players.begin() + i)
                // {
                sf::Text amountText(std::to_string(player.resources.at(Resource(j))), font);
                amountText.setCharacterSize(3 * gSH);
                amountText.setOutlineThickness(1);
                amountText.setOrigin(playerBackgroundOffset);
                auto textRect = amountText.getLocalBounds();
                amountText.setPosition(sf::Vector2f(6 * gSW + (3 + j) * (3 * gSH + gSW) + 1.5 * gSH - textRect.width / 2, 5 * gSH));
                window->draw(amountText);
                // }
            }
            // Cards
            for (auto card : player.cardRects)
                window->draw(card);
            // Buttons
            for (auto button : gameButtons)
                window->draw(button);
        }
    }
    void drawMap()
    {
        drawTiles();
        drawNumbers();
        drawRobber();
        drawBuildings();
        drawBuildingSpots();
    }
    void drawTiles()
    {
        for (size_t y = 0; y < map.height; y++)
        {
            for (size_t x = 0; x < map.width; x++)
            {
                Tile *tile = &map.tileMap[x][y];
                if (tile->type != none)
                {
                    sf::CircleShape bgTileShape(bgTileSize, 6);
                    sf::CircleShape tileShape(tileSize, 6);

                    bgTileShape.setOrigin(bgTileSize, bgTileSize);
                    tileShape.setOrigin(tileSize, tileSize);

                    sf::Vector2f position(
                        bgTileWidth / 2 + x * bgTileWidth + ((y + 1) % 2) * (bgTileWidth / 2),
                        bgTileHeight / 2 + y * (3 * (bgTileHeight / 4)));
                    tile->windowPosition = position;
                    bgTileShape.setPosition(position);
                    tileShape.setPosition(position);

                    if (tile->type == ocean)
                        bgTileShape.setFillColor(sf::Color(0, 157, 174));
                    else
                        bgTileShape.setFillColor(sf::Color::White);

                    tileShape.setTexture(&tileTextures);
                    tileShape.setTextureRect(sf::IntRect(((tile->type - 1) % 4) * textureSize, (((tile->type - 1) / 4) % 4) * textureSize, textureSize, textureSize));

                    window->draw(bgTileShape);
                    window->draw(tileShape);
                }
            }
        }
    }
    void drawNumbers()
    {
        for (size_t i = 0; i < numberShapes.size(); i++)
        {
            auto shape = numberShapes[i];
            int diceSum = this->diceRolls[0] + this->diceRolls[1];
            if (robberActive)
                shape.setFillColor(spotColor);
            else if (diceSum == numberTiles[i].number)
                shape.setFillColor(sf::Color::Green);
            window->draw(shape);
        }
        for (auto text : numberTexts)
            window->draw(text);
    }
    void drawBuildings()
    {
        for (auto row : map.edgesMap)
            for (auto building : row)
                if (building.isBuilt)
                    window->draw(building.rect);

        for (auto row : map.verticesMap)
            for (auto building : row)
                if (building.isBuilt)
                    window->draw(building.rect);
    }
    void drawBuildingSpots()
    {
        if (turnState == build)
            for (auto spot : availableSpots)
                window->draw(spot->circle);
    }
    void drawRobber()
    {
        auto pos = robberPos;
        sf::RectangleShape robber(sf::Vector2f(7 * gSH, 7 * gSH));
        robber.setTexture(&robberTexture);
        robber.setFillColor(sf::Color(138, 138, 138));
        robber.setOrigin(sf::Vector2f(robber.getGlobalBounds().width / 2, robber.getGlobalBounds().width / 2));
        robber.setPosition(sf::Vector2f(bgTileWidth / 2 + pos.x * bgTileWidth + ((pos.y + 1) % 2) * (bgTileWidth / 2) + bgTileWidth / 4, bgTileHeight / 2 + pos.y * (3 * (bgTileHeight / 4))));
        window->draw(robber);
    }

    void playDevCard(CardType type)
    {
        switch (type)
        {
        case knight:
            robberActive = true;
            break;
        case roadBuilding:
            break;
        case yearOfPlenty:
            break;
        case monopoly:
            break;
        case victoryPoint:
            currentPlayer->points += 1;
            break;
        }
    }
    void generateCard()
    {
        auto type = *(currentPlayer->cards.end() - 1);
        auto index = currentPlayer->id;
        sf::RectangleShape card(sf::Vector2f(3 * gSH, 3 * gSH));
        card.setTexture(&devCardTexture);
        card.setTextureRect(sf::IntRect(0, 0, textureSize, textureSize));

        sf::Vector2f playerBackgroundOffset((int)-window->getSize().x + gameStateWidth - 5 * gSW, -gameStateHeight / 2 + 23 * gSH - index * (playerBackgroundSize.y + 2 * gSH));
        card.setOrigin(playerBackgroundOffset);
        card.setPosition(sf::Vector2f(6 * gSW + (currentPlayer->cards.size() - 1) * 4 * gSH + 8 * (3 * gSH + gSW), 9 * gSH));

        switch (type)
        {
        case knight:
            card.setFillColor(sf::Color::Red);
            break;
        case roadBuilding:
            break;
        case yearOfPlenty:
            break;
        case monopoly:
            break;
        case victoryPoint:
            card.setFillColor(sf::Color::Yellow);
            break;
        }
        currentPlayer->cardRects.push_back(card);
    }
    void buyDevCard()
    {
        if (canBuy(devCard))
        {
            int sum = 0;
            for (auto item : map.cardDistribution)
                sum += item.second;
            if (sum == 0)
                return;

            std::uniform_int_distribution<std::mt19937::result_type> dist(1, sum);
            auto index = dist(rng);
            sum = 0;
            for (auto item : map.cardDistribution)
            {
                if (item.second <= 0)
                    continue;
                if (sum + item.second >= index)
                {
                    map.cardDistribution.at(item.first) -= 1;
                    currentPlayer->cards.push_back(item.first);
                    generateCard();
                    break;
                }
                sum += item.second;
            }
            payForProduct(devCard);
        }
    }
    void generatePlayerBackgrounds()
    {
        sf::Vector2f playerBackgroundSize(90 * gSW, 13 * gSH);
        sf::RectangleShape playerBackground(playerBackgroundSize);
        playerBackground.setOrigin(backgroundCenterOffset);

        for (size_t i = 0; i < players.size(); i++)
        {
            auto player = players[i];
            playerBackground.setPosition(sf::Vector2f(-45 * gSW, -23 * gSH + i * (playerBackgroundSize.y + 2 * gSH)));
            sf::Vector2f playerBackgroundOffset((int)-window->getSize().x + gameStateWidth - 5 * gSW, -gameStateHeight / 2 + 23 * gSH - i * (playerBackgroundSize.y + 2 * gSH));
            playerBackgroundsRects.push_back(playerBackground);
        }
    }
    void generateNumbers()
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
                    numberShape.setFillColor(sf::Color::White);

                    numberText.setFont(this->font);
                    numberText.setFillColor(sf::Color::Black);
                    numberText.setStyle(sf::Text::Bold);
                    numberText.setString(std::to_string(tile.number));
                    numberText.setCharacterSize(std::floor(numberSize));

                    sf::FloatRect textSize = numberText.getLocalBounds();
                    numberText.setOrigin(textSize.width / 2, textSize.height);
                    numberText.setPosition(position);

                    numberShapes.push_back(numberShape);
                    numberTiles.push_back(tile);
                    numberTexts.push_back(numberText);
                }
            }
        }
    }
    void robPlayer(sf::Vector2f mousePos)
    {
        for (size_t i = 0; i < playerBackgroundsRects.size(); i++)
        {
            bool canBeRobbed = false;
            for (auto player : playersToBeRobbed)
                if (players[i].id == player->id)
                    canBeRobbed = true;
            if (!canBeRobbed)
                continue;
            if (playerBackgroundsRects[i].getGlobalBounds().contains(mousePos))
            {
                std::vector<size_t> indexes;
                for (size_t j = 0; j < players[i].resources.size(); j++)
                    if (players[i].resources.at(Resource(j)) > 0)
                        indexes.push_back(j);

                std::uniform_int_distribution<std::mt19937::result_type> dist(0, indexes.size() - 1);
                auto index = indexes[dist(rng)];
                players[i].resources[Resource(index)] -= 1;
                currentPlayer->resources[Resource(index)] += 1;
                robbingActive = false;
                break;
            }
        }
    }
    void placeRobber(sf::Vector2f mousePos)
    {
        for (size_t i = 0; i < numberShapes.size(); i++)
        {
            auto shape = numberShapes[i];
            if (shape.getGlobalBounds().contains(mousePos))
            {
                robberActive = false;
                robberPos = numberTiles[i].position;

                auto adjSpots = getAdjacentSpots(map.tileMap[robberPos.x][robberPos.y]);
                std::vector<Player *> potentialPlayers;
                playersToBeRobbed.clear();
                for (auto spot : adjSpots)
                    if (spot->type != road && spot->isBuilt)
                        potentialPlayers.push_back(spot->owner);
                for (auto player : potentialPlayers)
                {
                    bool canBeRobbed = false;
                    for (size_t j = 0; j < player->resources.size(); j++)
                        if (player->resources.at(Resource(j)) > 0)
                        {
                            canBeRobbed = true;
                            break;
                        }
                    if (canBeRobbed && player->id != currentPlayer->id)
                        playersToBeRobbed.push_back(player);
                }
                gatherResources();
                robbingActive = true;
                break;
            }
        }
    }
    void generateDice()
    {
        diceRects.clear();
        for (size_t i = 0; i < diceRolls.size(); i++)
        {
            auto value = diceRolls[i] - 1;
            sf::RectangleShape diceFace(sf::Vector2f(18 * gSH, 18 * gSH));
            diceFace.setOrigin(backgroundCenterOffset.x, backgroundCenterOffset.y + 48 * gSH);
            diceFace.setPosition(-42 * gSW + i * 43.5 * gSW, 1 * gSH);
            diceFace.setTexture(&diceTextures);
            diceFace.setTextureRect(sf::IntRect((value % 3) * textureSize, ((value / 3) % 3) * textureSize, textureSize, textureSize));
            diceRects.push_back(diceFace);
        }
    }
    void generateButtons()
    {
        sf::Vector2f backgroundBottomLeftOffset((int)-window->getSize().x + gameStateWidth, -gameStateHeight);
        int buttonCount = 7;
        int buttonSpacing = 3 * gSW;
        int buttonWidth = (90 * gSW - (buttonCount - 3) * buttonSpacing) / (buttonCount - 2);
        for (size_t j = 0; j < buttonCount; j++)
        {
            sf::Vector2f buttonSize(buttonWidth, buttonWidth);
            sf::RectangleShape button(buttonSize);
            button.setOrigin(backgroundCenterOffset);
            button.setPosition(sf::Vector2f(-45 * gSW + j * (buttonWidth + buttonSpacing), 48 * gSH - buttonWidth));
            switch (j)
            {
            case 0:
            case 1:
            case 2:
                button.setTexture(&buildingsTextures);
                button.setTextureRect(sf::IntRect(j * textureSize, 0, textureSize, textureSize));
                button.setFillColor(colorPalette[2]);
                break;
            case 3:
                button.setTexture(&devCardTexture);
                button.setTextureRect(sf::IntRect(0, 0, textureSize, textureSize));
                button.setFillColor(colorPalette[2]);
                break;
            case 4:
                button.setFillColor(colorPalette[2]);
                break;
            case 5:
                buttonWidth += 10 * gSW;
                button.setSize(sf::Vector2f(buttonWidth, buttonWidth));
                button.setOrigin(backgroundBottomLeftOffset);
                button.setPosition(sf::Vector2f(-5 * gSW - buttonWidth, -2 * gSH - buttonWidth));
                button.setFillColor(colorPalette[3]);
                button.setOutlineThickness(gSW);
                button.setOutlineColor(sf::Color::White);
                break;
            case 6:
                button.setSize(sf::Vector2f(buttonWidth - 2 * gSH, buttonWidth - 2 * gSH));
                button.setOrigin(backgroundBottomLeftOffset);
                button.setPosition(sf::Vector2f(-5 * gSW - buttonWidth + 1 * gSH, -1 * gSH - buttonWidth));
                button.setTexture(&playTexture);
                button.setTextureRect(sf::IntRect(0, 0, textureSize, textureSize));
                button.setFillColor(sf::Color::White);
                break;
            }
            gameButtons.push_back(button);
        }
    }
    void gatherResources()
    {
        int diceSum = diceRolls[0] + diceRolls[1];
        if (diceSum == 7 && !setupEnabled)
            return;
        for (auto tileRow : map.tileMap)
            for (auto tile : tileRow)
            {
                if (setupEnabled || tile.number == diceSum && sf::Vector2u(tile.position.x, tile.position.y) != robberPos)
                {
                    auto adjacentSpots = getAdjacentSpots(tile);
                    for (auto spot : adjacentSpots)
                    {
                        if (spot->isBuilt && spot->type != road && tile.type != none && tile.type != ocean && tile.type != desert)
                        {
                            spot->owner->resources[TileProduct[tile.type]] += spot->level;
                        }
                    }
                }
            }
    }
    void endTurn()
    {
        if (setupEnabled)
        {
            if (setupSecondTurn)
                if (currentPlayer - 1 >= players.begin())
                    currentPlayer--;
                else
                {
                    gatherResources();
                    setupEnabled = false;
                    setupSecondTurn = false;
                    turnState = dice;
                    return;
                }
            else if (currentPlayer + 1 < players.end())
                currentPlayer++;
            else
                setupSecondTurn = true;
            chooseProduct(settlement);
        }
        else if (turnState != dice && !robbingActive && !robberActive)
        {
            currentPlayer = (currentPlayer + 1 < players.end()) ? currentPlayer + 1 : players.begin();
            turnState = dice;
        }
    }
    bool canBuy(Product type)
    {
        if (turnState == dice)
            return false;
        if (type != devCard && currentPlayer->buildings[type] <= 0)
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
        if (canBuy(chosenProduct) || setupEnabled)
        {
            turnState = build;
            getAvailableSpots(chosenProduct);
        }
    }
    void payForProduct(Product type)
    {
        if (!setupEnabled)
        {
            auto cost = ProductCost[type];
            for (auto pair : cost)
                currentPlayer->resources[pair.first] -= pair.second;
        }
        if (type != devCard)
            currentPlayer->buildings[type] -= 1;
    }
    void nextGameState()
    {
        gameState = GameState((gameState + 1) % 4);
    }
    void setMap(std::string mapName)
    {
        map.loadFromFile(mapName);
        generatePlayerBackgrounds();
        generateNumbers();
        robberPos = map.robberPos;
        generateBuildingSpots();
        chosenProduct = settlement;
        getAvailableSpots(chosenProduct);
    }
    void rollTheDice()
    {
        std::uniform_int_distribution<std::mt19937::result_type> diceDist(1, 6);
        std::vector<std::mt19937::result_type> diceRolls = {diceDist(rng), diceDist(rng)};
        this->diceRolls = diceRolls;
        generateDice();
        gatherResources();
        if (diceRolls[0] + diceRolls[1] == 7)
            robberActive = true;
        turnState = idle;
    }
    void placeBuilding(Product type, sf::Vector2f mousePos)
    {
        for (auto spot : availableSpots)
        {
            if (spot->circle.getGlobalBounds().contains(mousePos))
            {
                if (type == settlement)
                {
                    spot->type = type;
                    spot->isBuilt = true;
                    spot->owner = &(*currentPlayer);
                    sf::RectangleShape building(sf::Vector2f(buildingSize, buildingSize));
                    building.setOrigin(sf::Vector2f(buildingSize / 2, buildingSize / 2));
                    building.setPosition(spot->circle.getGlobalBounds().left + spot->circle.getGlobalBounds().width / 2, spot->circle.getGlobalBounds().top + spot->circle.getGlobalBounds().height / 2);
                    building.setTexture(&buildingsTextures);
                    building.setTextureRect(sf::IntRect(0, 0, textureSize, textureSize));
                    building.setFillColor(currentPlayer->color);
                    spot->rect = building;
                    currentPlayer->points += 1;
                    placedCityPos = spot->position;
                }
                if (type == city)
                {
                    spot->type = type;
                    spot->level = 2;
                    auto &building = spot->rect;
                    building.setTextureRect(sf::IntRect(textureSize, 0, textureSize, textureSize));
                    currentPlayer->points += 1;
                }
                if (type == road)
                {
                    spot->type = type;
                    spot->isBuilt = true;
                    spot->owner = &(*currentPlayer);
                    sf::RectangleShape building(sf::Vector2f(this->buildingSize, this->buildingSize / 4));
                    building.setOrigin(sf::Vector2f(this->buildingSize / 2, this->buildingSize / 4 / 2));
                    building.setPosition(spot->circle.getGlobalBounds().left + spot->circle.getGlobalBounds().width / 2, spot->circle.getGlobalBounds().top + spot->circle.getGlobalBounds().height / 2);
                    building.setTexture(&buildingsTextures);
                    building.setTextureRect(sf::IntRect(textureSize * 2, 3 * textureSize / 4, textureSize, textureSize / 4));
                    building.setFillColor(currentPlayer->color);
                    sf::Vector2u pos = spot->position;
                    building.setRotation((pos.y % 2) ? 90 : (pos.y % 4) ? pos.x % 2 ? 30 : -30
                                                        : pos.x % 2     ? -30
                                                                        : 30);
                    spot->rect = building;
                }
                payForProduct(chosenProduct);
                turnState = idle;
                if (setupEnabled && type == settlement)
                    chooseProduct(road);
                else if (setupEnabled && type == road)
                    endTurn();
            }
        }
    }
    sf::Text centerText(std::string txt)
    {
        auto size = window->getSize();
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
        for (size_t x = 0; x < map.verticesMap.size(); x += 1)
            for (size_t y = 0; y < map.verticesMap[x].size(); y += 1)
            {
                Building *spot = &map.verticesMap[x][y];
                spot->position = sf::Vector2u(x, y);
                sf::CircleShape *circle = &spot->circle;
                circle->setRadius(spotSize);
                circle->setFillColor(spotColor);
                circle->setOutlineColor(sf::Color::Black);
                circle->setOutlineThickness(spotSize / 10);
                circle->setOrigin(sf::Vector2f(spotSize, spotSize));
                sf::Vector2f position(
                    x * bgTileWidth / 2,
                    y * bgTileHeight * 3 / 4 + ((x % 2 + y % 2) % 2) * bgTileHeight / 4);
                spot->windowPosition = position;
                circle->setPosition(position);

                auto tiles = getAdjacentTiles(*spot);
                for (auto tile : tiles)
                    if (tile->isLand())
                        spot->isLand = true;
            }
        for (size_t x = 0; x < map.edgesMap.size(); x += 1)
            for (size_t y = 0; y < map.edgesMap[x].size(); y += 1)
            {
                Building *spot = &map.edgesMap[x][y];
                spot->position = sf::Vector2u(x, y);
                sf::CircleShape *circle = &spot->circle;
                circle->setRadius(spotSize);
                circle->setFillColor(spotColor);
                circle->setOutlineColor(sf::Color::Black);
                circle->setOutlineThickness(spotSize / 10);
                circle->setOrigin(sf::Vector2f(spotSize, spotSize));
                sf::Vector2f position(
                    x * bgTileWidth / ((!(y % 2) + 1)) + !(y % 2) * bgTileWidth / 4 + (y % 4 == 1) * (y % 2) * bgTileWidth / 2,
                    y * bgTileHeight * 3 / 8 + bgTileHeight / 8);
                spot->windowPosition = position;
                circle->setPosition(position);

                auto tiles = getAdjacentTiles(*spot);
                for (auto tile : tiles)
                    if (tile->isLand())
                        spot->isLand = true;
            }
    }
    void getAvailableSpots(Product type)
    {
        availableSpots.clear();

        std::vector<Building> playerCities;
        std::vector<Building> playerRoads;
        for (auto row : map.verticesMap)
            for (auto spot : row)
                if (spot.isBuilt && spot.owner->id == currentPlayer->id)
                    playerCities.push_back(spot);
        for (auto row : map.edgesMap)
            for (auto spot : row)
                if (spot.isBuilt && spot.owner->id == currentPlayer->id)
                    playerRoads.push_back(spot);

        if (type == settlement)
        {
            if (setupEnabled)
                for (size_t x = 0; x < map.verticesMap.size(); x += 1)
                    for (size_t y = 0; y < map.verticesMap[x].size(); y += 1)
                    {
                        Building *s = &map.verticesMap[x][y];
                        if (s->isLand)
                        {
                            auto adjacentRoads = getAdjacentRoads(*s);
                            bool canBuild = true;
                            for (auto road2 : adjacentRoads)
                            {
                                auto spots = getAdjacentCities(*road2);
                                for (auto spot : spots)
                                {
                                    if (spot->isBuilt)
                                        canBuild = false;
                                }
                            }
                            if (canBuild)
                            {
                                bool addSpot = true;
                                for (auto x : availableSpots)
                                    if (x->position == s->position)
                                        addSpot = false;
                                if (addSpot)
                                    availableSpots.push_back(s);
                            }
                        }
                    }
            else
            {
                std::vector<Building *> candidateSpots;
                for (auto road : playerRoads)
                {
                    auto adjacentSpots = getAdjacentCities(road);
                    for (auto spot : adjacentSpots)
                        if (!spot->isBuilt)
                            candidateSpots.push_back(spot);
                    for (auto candidateSpot : candidateSpots)
                    {
                        auto adjacentRoads = getAdjacentRoads(*candidateSpot);
                        bool canBuild = true;
                        for (auto road2 : adjacentRoads)
                        {
                            auto spots = getAdjacentCities(*road2);
                            for (auto spot : spots)
                            {
                                if (spot->isBuilt)
                                    canBuild = false;
                            }
                        }
                        if (canBuild)
                        {
                            bool addSpot = true;
                            for (auto x : availableSpots)
                                if (x->position == candidateSpot->position)
                                    addSpot = false;
                            if (addSpot)
                                availableSpots.push_back(candidateSpot);
                        }
                    }
                }
            }
        }
        if (type == city)
        {
            for (size_t x = 0; x < map.verticesMap.size(); x += 1)
                for (size_t y = 0; y < map.verticesMap[x].size(); y += 1)
                {
                    Building *spot = &map.verticesMap[x][y];
                    if (spot->isBuilt && spot->level == 1 && spot->owner->id == currentPlayer->id)
                        availableSpots.push_back(spot);
                }
        }
        if (type == road)
        {
            if (setupEnabled)
            {
                auto spots = getAdjacentRoads(map.verticesMap[placedCityPos.x][placedCityPos.y]);
                for (auto spot : spots)
                    if (!spot->isBuilt && spot->isLand)
                        availableSpots.push_back(spot);
            }
            else
            {
                for (auto city : playerCities)
                {
                    auto spots = getAdjacentRoads(city);
                    for (auto spot : spots)
                        if (!spot->isBuilt && spot->isLand)
                        {
                            bool addSpot = true;
                            for (auto x : availableSpots)
                                if (x->position == spot->position)
                                    addSpot = false;
                            if (addSpot)
                                availableSpots.push_back(spot);
                        }
                }
                for (auto road : playerRoads)
                {
                    auto spots = getAdjacentRoads(road);
                    for (auto spot : spots)
                        if (!spot->isBuilt && spot->isLand)
                        {
                            bool addSpot = true;
                            for (auto x : availableSpots)
                                if (x->position == spot->position)
                                    addSpot = false;
                            if (addSpot)
                                availableSpots.push_back(spot);
                        }
                }
            }
        }
    }
    bool isOffLimits(sf::Vector2u pos)
    {
        if (pos.x < 0 || pos.x > map.width - 1 || pos.y < 0 || pos.y > map.height - 1)
            return true;
        return false;
    }
    std::vector<Tile *> getAdjacentTiles(Building spot)
    {
        std::vector<Tile *> adjacentTiles;
        size_t x = spot.position.x;
        size_t y = spot.position.y;
        sf::Vector2u pos;
        if (spot.type == settlement || spot.type == city)
        {
            pos = {x / 2, y - !((x + y) % 2)};
            if (!isOffLimits(pos))
                adjacentTiles.push_back(&map.tileMap[pos.x][pos.y]);
            pos = {x / 2 - 1, y - !((x + y) % 2)};
            if (!isOffLimits(pos))
                adjacentTiles.push_back(&map.tileMap[pos.x][pos.y]);
            pos = {x / 2 - !(x % 2), y - ((x + y) % 2)};
            if (!isOffLimits(pos))
                adjacentTiles.push_back(&map.tileMap[pos.x][pos.y]);
        }
        if (spot.type == road)
        {
            pos = {y % 2 ? x - 1 : (x - 1) / 2, y % 2 ? y / 2 : y % 4 == 0 ? y / 2
                                                                           : (y - 1) / 2};
            if (!isOffLimits(pos))
                adjacentTiles.push_back(&map.tileMap[pos.x][pos.y]);
            pos = {y % 2 ? x : x / 2, y % 2 ? y / 2 : y % 4 == 0 ? (y - 1) / 2
                                                                 : y / 2};
            if (!isOffLimits(pos))
                adjacentTiles.push_back(&map.tileMap[pos.x][pos.y]);
        }
        return adjacentTiles;
    }
    std::vector<Building *> getAdjacentSpots(Tile tile)
    {
        std::vector<Building *> adjacentSpots;
        size_t x = tile.position.x;
        size_t y = tile.position.y;

        adjacentSpots.push_back(
            &map.verticesMap[y % 2 ? 2 * x : 2 * x + 1][y]);
        adjacentSpots.push_back(
            &map.verticesMap[y % 2 ? 2 * x + 1 : 2 * x + 2][y]);
        adjacentSpots.push_back(
            &map.verticesMap[y % 2 ? 2 * x + 2 : 2 * x + 3][y]);
        adjacentSpots.push_back(
            &map.verticesMap[y % 2 ? 2 * x : 2 * x + 1][y + 1]);
        adjacentSpots.push_back(
            &map.verticesMap[y % 2 ? 2 * x + 1 : 2 * x + 2][y + 1]);
        adjacentSpots.push_back(
            &map.verticesMap[y % 2 ? 2 * x + 2 : 2 * x + 3][y + 1]);

        adjacentSpots.push_back(
            &map.edgesMap[y % 2 ? 2 * x : 2 * x + 1][y * 2]);
        adjacentSpots.push_back(
            &map.edgesMap[y % 2 ? 2 * x + 1 : 2 * x + 2][y * 2]);
        adjacentSpots.push_back(
            &map.edgesMap[x][y * 2 + 1]);
        adjacentSpots.push_back(
            &map.edgesMap[x + 1][y * 2 + 1]);
        adjacentSpots.push_back(
            &map.edgesMap[y % 2 ? 2 * x : 2 * x + 1][y * 2 + 2]);
        adjacentSpots.push_back(
            &map.edgesMap[y % 2 ? 2 * x + 1 : 2 * x + 2][y * 2 + 2]);

        return adjacentSpots;
    }
    std::vector<Building *> getAdjacentRoads(Building spot)
    {
        std::vector<Building *> adjacentSpots;
        size_t x = spot.position.x;
        size_t y = spot.position.y;
        if (spot.type == settlement || spot.type == city)
        {
            adjacentSpots.push_back(
                &map.edgesMap[x][y * 2]);
            adjacentSpots.push_back(
                &map.edgesMap[x - 1][y * 2]);
            adjacentSpots.push_back(
                &map.edgesMap[x / 2][(x + y) % 2 ? y * 2 + 1 : y * 2 - 1]);
        }
        if (spot.type == road)
        {
            adjacentSpots.push_back(
                &map.edgesMap[y % 2 ? (y - 1) % 4 ? 2 * x - 1 : 2 * x + 1 : (y % 4) ? x / 2
                                                                                    : (x + 1) / 2][y % 2 ? y - 1 : y - 1]);
            adjacentSpots.push_back(
                &map.edgesMap[y % 2 ? 2 * x : x - 1][y % 2 ? y - 1 : y]);
            adjacentSpots.push_back(
                &map.edgesMap[y % 2 ? (y - 1) % 4 ? 2 * x - 1 : 2 * x + 1 : x + 1][y % 2 ? y + 1 : y]);
            adjacentSpots.push_back(
                &map.edgesMap[y % 2 ? 2 * x : (y % 4) ? (x + 1) / 2
                                                      : x / 2][y % 2 ? y + 1 : y + 1]);
        }
        return adjacentSpots;
    }
    std::vector<Building *> getAdjacentCities(Building spot)
    {
        std::vector<Building *> adjacentSpots;
        size_t x = spot.position.x;
        size_t y = spot.position.y;
        if (spot.type == settlement || spot.type == city)
        {
            adjacentSpots.push_back(
                &map.verticesMap[x - 1][y]);
            adjacentSpots.push_back(
                &map.verticesMap[x + 1][y]);
            adjacentSpots.push_back(
                &map.verticesMap[x][(x + y) % 2 ? y + 1 : y - 1]);
        }
        if (spot.type == road)
        {
            adjacentSpots.push_back(
                &map.verticesMap[y % 2 ? (y - 1) % 4 ? 2 * x : 2 * x + 1 : x][y % 2 ? y / 2 : y / 2]);
            adjacentSpots.push_back(
                &map.verticesMap[y % 2 ? (y - 1) % 4 ? 2 * x : 2 * x + 1 : x + 1][y % 2 ? y / 2 + 1 : y / 2]);
        }
        return adjacentSpots;
    }
    void processMouseClick(sf::Vector2f mousePos)
    {
        if (!setupEnabled && !robberActive)
        {
            if (isPressed(diceRects[0], mousePos) || isPressed(diceRects[1], mousePos))
                rollTheDice();
            if (isPressed(gameButtons[0], mousePos))
                chooseProduct(settlement);
            if (isPressed(gameButtons[1], mousePos))
                chooseProduct(city);
            if (isPressed(gameButtons[2], mousePos))
                chooseProduct(road);
            if (isPressed(gameButtons[3], mousePos))
                buyDevCard();
            if (isPressed(gameButtons[4], mousePos))
                std::cout << 4 << "\n";
            if (isPressed(gameButtons[5], mousePos))
                endTurn();
            for (size_t i = 0; i < currentPlayer->cards.size(); i++)
                if (isPressed(currentPlayer->cardRects[i], mousePos))
                    playDevCard(currentPlayer->cards[i]);
        }
        if (robberActive)
            placeRobber(mousePos);
        if (robbingActive)
            robPlayer(mousePos);
        if (turnState == build)
            placeBuilding(chosenProduct, mousePos);
    }
    bool isPressed(sf::RectangleShape rect, sf::Vector2f pos)
    {
        return rect.getGlobalBounds().contains(pos);
    }

    void debug()
    {
        sf::Vector2f pos(0, 0);
        if (drawCoordinates)
        {
            for (auto tileRow : map.tileMap)
                for (auto tile : tileRow)
                {
                    sf::Text txt(std::to_string(tile.position.x) + "," + std::to_string(tile.position.y), font);
                    txt.setCharacterSize(25);
                    txt.setOutlineThickness(1);
                    txt.setOrigin(sf::Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
                    txt.setPosition(tile.windowPosition);
                    window->draw(txt);
                }
            for (auto spotRow : map.verticesMap)
                for (auto spot : spotRow)
                {
                    sf::Text txt(std::to_string(spot.position.x) + "," + std::to_string(spot.position.y), font);
                    txt.setCharacterSize(25);
                    txt.setOutlineThickness(1);
                    txt.setOrigin(sf::Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
                    txt.setPosition(spot.windowPosition);
                    // for (auto testSpot : test)
                    //     if (testSpot.position == spot.position && testSpot.type == spot.type)
                    //         txt.setFillColor(sf::Color::Red);
                    window->draw(txt);
                }
            for (auto spotRow : map.edgesMap)
                for (auto spot : spotRow)
                {
                    sf::Text txt(std::to_string(spot.position.x) + "," + std::to_string(spot.position.y), font);
                    txt.setCharacterSize(25);
                    txt.setOutlineThickness(1);
                    txt.setOrigin(sf::Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
                    txt.setPosition(spot.windowPosition);
                    // for (auto testSpot : test)
                    //     if (testSpot.position == spot.position && testSpot.type == spot.type)
                    //         txt.setFillColor(sf::Color::Red);
                    window->draw(txt);
                }
        }
        pos = printAndMoveDebugLine(std::to_string(window->getSize().x) + "x" + std::to_string(window->getSize().y), pos);
        pos = printAndMoveDebugLine("Map name: " + map.name, pos);
        pos = printAndMoveDebugLine("", pos);
        pos = printAndMoveDebugLine("Game State: " + std::to_string(gameState), pos);
        pos = printAndMoveDebugLine("Turn State: " + std::to_string(turnState), pos);
        pos = printAndMoveDebugLine("Current player: " + std::to_string(currentPlayer->id), pos);
        pos = printAndMoveDebugLine("", pos);
        pos = printAndMoveDebugLine("Setup enabled: " + std::to_string(setupEnabled), pos);
        pos = printAndMoveDebugLine("Setup second turn: " + std::to_string(setupSecondTurn), pos);
    }
    sf::Vector2f printAndMoveDebugLine(std::string txt, sf::Vector2f pos)
    {
        sf::Text text(txt, font);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
        text.setStyle(sf::Text::Bold);
        text.setPosition(pos);
        window->draw(text);
        pos.y += text.getCharacterSize();
        return pos;
    }
};

#endif