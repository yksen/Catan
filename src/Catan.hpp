#ifndef Catan_H
#define Catan_H
#include "Map.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

enum Stage { menu, settings, game };

class Catan {
    private:
        sf::RenderWindow* window;
        Stage stage;
        Map map;  

    public:        
        Catan(sf::RenderWindow* x) {
            window = x;
            stage = menu;
        }
        
        Stage getStage() { return stage; }
        void setMap(std::string mapName) { map.setMap(mapName); }

        void drawGame()
        {
            window->clear();
            window->draw(sf::CircleShape(80, 6));
            window->display();
        }  
    
};

#endif