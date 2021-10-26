#ifndef Map_H
#define Map_H
#include "Hex.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Map {
    private:
        std::string name;
        
    public: 
        void setMap(std::string mapName) {
            name = mapName;
            std::string line;
            std::string filePath = "../maps/" + mapName + ".txt";
            std::ifstream file(filePath);
            if (file.is_open())
            {
                while (getline(file, line))
                {
                    std::cout << line << "\n";
                }
                file.close();
            }
        }
};

#endif