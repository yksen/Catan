#ifndef Map_H
#define Map_H
#include "Tile.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

class Map {
    private:
        std::string name;                
        
    public: 
        size_t width, height;
        std::vector<std::vector<Tile>> tileMap;
        
        void setMap(std::string mapName) {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist(1, 5);

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
                            tileMap[x][y] = Tile(x, y, ocean, 0);
                        }
                        else
                        {
                            tileMap[x][y] = Tile(x, y, TileType(dist(rng)), 7);
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