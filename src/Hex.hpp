#ifndef Hex_H
#define Hex_H

enum TileType { hill, forest, mountain, field, pasture };

class Hex {
    private:
        TileType tileType;
        int number;

    public: 
        Hex(TileType a, int b) {
            tileType = a;
            number = b;
        }
        
};

#endif