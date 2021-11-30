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
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::RShift)
                game.rollTheDice();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter)
                game.nextStage();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift)
                game.debug();
                // game.buildingsSpotsDrawingEnabled = !game.buildingsSpotsDrawingEnabled;
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                game.placeBuilding(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
        }

        game.draw();
    }
}
