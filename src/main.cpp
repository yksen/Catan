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

    Catan game(&window);
    game.setMap("Standard");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                game.processMouseClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F3)
            {
                if (!game.debugingEnabled)
                    game.debugingEnabled = !game.debugingEnabled;
                else if (!game.drawCoordinates)
                    game.drawCoordinates = !game.drawCoordinates;
                else
                {
                    game.drawCoordinates = !game.drawCoordinates;
                    game.debugingEnabled = !game.debugingEnabled;
                }
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter)
                game.nextGameState();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift)
                game.endTurn();
        }

        game.draw();
    }
}
