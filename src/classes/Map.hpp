#ifndef Map_H
#define Map_H

#include "Tile.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> typeDist(1, 6);
std::uniform_int_distribution<std::mt19937::result_type> numberDist(2, 12);

class Map {
    private:
        std::string name;                
        
    public: 
        size_t width, height;
        std::vector<std::vector<Tile>> tileMap;
        
        void setMap(std::string mapName) {            
            name = mapName;
            std::string line, filePath = "../assets/maps/" + mapName + ".txt";
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
                            tileMap[x][y] = Tile(x, y, TileType(typeDist(rng)), numberDist(rng));
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