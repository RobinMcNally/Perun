#ifndef WORLD_H
#define WORLD_H value

const int WORLD_WIDTH = 50;
const int WORLD_HEIGHT = 50;

//Starting with two dimension to begin the dungeon
class world {
    public:
    int ** worldmap;
        void generate_world();
        int ** get_world_map();
};

#endif
