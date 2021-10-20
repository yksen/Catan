#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

void rendering_thread(sf::RenderWindow* window)
{
    window->setActive(true);

    while (window->isOpen())
    {
        window->clear();
        window->draw(sf::CircleShape(80, 6));
        window->display();
    }
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Catan", sf::Style::Fullscreen, settings);
    window.setActive(false);

    sf::Thread thread(&rendering_thread, &window);
    thread.launch();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
        }
        
    }
}