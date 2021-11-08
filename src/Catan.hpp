#ifndef Catan_H
#define Catan_H
#include "Map.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

enum Stage { menu, settings, game };
std::map<TileType, sf::Color> TileColors { {ocean, sf::Color::Blue} };

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
            drawMap();
            window->display();
        }  

        void drawMap()
        {
            size_t x = 0, y = 0;

            for (y; y < map.height; y++)
            {
                for (x; x < map.width; x++)
                {
                    sf::CircleShape tile(80, 6);
                    window->draw(tile);
                }
            }
        }
    
};

#endif