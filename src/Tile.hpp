#ifndef Tile_H
#define Tile_H

enum TileType { ocean, hill, forest, mountain, field, pasture };

class Tile {
    private:
        TileType type;
        int number;

    public: 
        Tile(TileType a, int b) {
            type = a;
            number = b;
        }
        
};

#endif