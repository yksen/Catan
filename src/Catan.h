#ifndef Catan_H
#define Catan_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Catan {
    private:
        enum Stage { menu, settings, game };

        sf::RenderWindow* window;
        Stage stage;

    public:        
        Catan(sf::RenderWindow* x) {
            window = x;
            stage = menu;
        }

        void drawGame()
        {
            window->clear();
            window->draw(sf::CircleShape(80, 6));
            window->display();
        }  
    
        Stage getStage()
        {
            return stage;
        }
};

#endif