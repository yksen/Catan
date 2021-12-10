#include "classes/Catan.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Catan", sf::Style::Fullscreen, settings);

    Catan game(&window, window.getSize());
    game.setMap("Standard");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
                game.setMap("Standard");
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::RShift && game.turnState == dice)
                game.rollTheDice();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter)
                game.nextGameState();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift)
                game.endTurn();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S)
            {
                game.turnState = build;
                game.chosenProduct = settlement;
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
            {
                game.turnState = build;
                game.chosenProduct = city;
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)
            {
                game.turnState = build;
                game.chosenProduct = road;
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F3)
                game.debug();
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                game.placeBuilding(game.chosenProduct, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));

            }
        }

        

        game.draw();
    }
}
