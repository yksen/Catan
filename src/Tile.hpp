#ifndef Tile_H
#define Tile_H

enum TileType { ocean, hill, forest, mountain, field, pasture, desert };

class Tile {
    private:
        int number;
        int x, y;

    public:
        TileType type;

        Tile(int a = -1, int b = -1, TileType c = ocean, int d = 7) {
            x = a;
            y = b;
            type = c;
            number = d;
        }
};

#endif