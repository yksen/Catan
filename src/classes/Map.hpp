#ifndef Map_H
#define Map_H

#include "Tile.hpp"
#include "Building.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <time.h>

std::mt19937 rng(time(0));

class Map
{
public:
    std::string name;
    size_t width, height;
    std::vector<std::pair<int, int>> typeDistribution;
    std::vector<std::pair<int, int>> numberDistribution;
    std::vector<std::vector<Tile>> tileMap;
    std::vector<std::vector<Building>> verticesMap;
    size_t vMapWidth, vMapHeight;
    std::vector<std::vector<Building>> edgesMap;
    size_t eMapWidth, eMapHeight;
    sf::Vector2u robberPos;

    void loadFromFile(std::string mapName)
    {
        this->name = mapName;
        std::string filePath = "../assets/maps/" + mapName + ".txt";
        this->typeDistribution.clear();
        this->numberDistribution.clear();
        this->tileMap.clear();
        this->verticesMap.clear();
        this->edgesMap.clear();

        std::ifstream file(filePath);
        if (file.is_open())
        {
            std::string line;
            std::vector<std::string> words;

            getline(file, line);
            words = splitWords(line);
            this->width = std::stoull(words[0]);
            this->height = std::stoull(words[1]);
            this->vMapWidth = 2*this->width + 1;
            this->vMapHeight = this->height + 1;
            this->eMapWidth = 2*this->width;
            this->eMapHeight = 2*this->height + 1;
            this->tileMap.resize(this->width, std::vector<Tile>(this->height));

            getline(file, line);
            words = splitWords(line);
            for (size_t i = 0; i < words.size(); i++)
                this->typeDistribution.push_back(std::pair<int, int>(int(i) + 2, std::stoi(words[i])));
            eraseZeros(&this->typeDistribution);

            getline(file, line);
            words = splitWords(line);
            for (size_t i = 0; i < words.size(); i++)
                this->numberDistribution.push_back(std::pair<int, int>(int(i) + 2, std::stoi(words[i])));
            eraseZeros(&this->numberDistribution);

            size_t x = 0, y = 0;
            while (getline(file, line))
            {
                x = 0;
                for (char c : line)
                {
                    if (c == '0')
                    {
                        this->tileMap[x][y] = Tile(x, y, ocean, 0);
                    }
                    else
                    {
                        std::uniform_int_distribution<std::mt19937::result_type> typeDist(0, this->typeDistribution.size() - 1);
                        std::uniform_int_distribution<std::mt19937::result_type> numberDist(0, this->numberDistribution.size() - 1);
                        auto randomType = typeDist(rng);
                        auto randomNumber = numberDist(rng);

                        if (TileType(this->typeDistribution[randomType].first) == desert)
                        {
                            this->tileMap[x][y] = Tile(x, y, TileType(this->typeDistribution[randomType].first), 0);
                            robberPos = {x, y};
                        }
                        else
                        {
                            this->tileMap[x][y] = Tile(x, y, TileType(this->typeDistribution[randomType].first), this->numberDistribution[randomNumber].first);
                            this->numberDistribution[randomNumber].second -= 1;
                            eraseZeros(&this->numberDistribution);
                        }
                        this->typeDistribution[randomType].second -= 1;
                        eraseZeros(&this->typeDistribution);
                    }
                    x++;
                }
                y++;
            }
            file.close();

            verticesMap.resize(vMapWidth);
            for (size_t i = 0; i < verticesMap.size(); i += 1)
                verticesMap[i].resize(vMapHeight, Building(settlement));

            edgesMap.resize(eMapWidth);
            for (size_t i = 0; i < edgesMap.size(); i += 1)
                edgesMap[i].resize(eMapHeight, Building(road));

        }
    }
    std::vector<std::string> splitWords(std::string line)
    {
        std::vector<std::string> words;
        size_t pos = 0;
        while ((pos = line.find(" ")) != std::string::npos)
        {
            words.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        return words;
    }
    void eraseZeros(std::vector<std::pair<int, int>> *v)
    {
        for (size_t i = 0; i < (*v).size(); i++)
        {
            if ((*v)[i].second == 0)
            {
                (*v).erase((*v).begin() + i);
                break;
            }
        }
    }
};

#endif