#ifndef Map_H
#define Map_H
#include "Tile.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Map {
    private:
        std::string name;                
        
    public: 
        size_t width, height;
        std::vector<std::vector<Tile>> tileMap;
        
        void setMap(std::string mapName) {
            name = mapName;
            std::string line, filePath = "../maps/" + mapName + ".txt";
            std::ifstream file(filePath);
            if (file.is_open())
            {
                tileMap.resize(20, std::vector<Tile>(20));
                size_t x = 0, y = 0;
                while (getline(file, line))
                {
                    x = 0;
                    for (char c : line)
                    {
                        if (c == '0')
                        {
                            tileMap[x][y] = Tile(ocean, 0);
                        }
                        else
                        {
                            tileMap[x][y] = Tile(hill, 7);
                        }     
                        x++;                   
                    }
                    y++;
                }                
                file.close();
                width = x;
                height = y;
            }
        }
};

#endif