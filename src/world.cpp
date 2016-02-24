#include "../include/world.h"
#include <time.h>
#include <cstdlib>

extern const int WORLD_WIDTH;
extern const int WORLD_HEIGHT;

int** drunkards_walk () {
    int ** worldmap= new int*[WORLD_HEIGHT];
    for (size_t i = 0; i < WORLD_HEIGHT; i++) {
        worldmap[i] = new int[WORLD_WIDTH]();
    }
    srand(time(NULL));

    int drunkard[2];
    drunkard[0] = WORLD_WIDTH/2;
    drunkard[1] = WORLD_HEIGHT/2;

    int stopcase = false;
    int countdown = 100;
    while (!stopcase) {
        worldmap[drunkard[0]][drunkard[1]] = 1;
        switch (rand() % 4) {
            case 0:
                drunkard[0]--;
                break;
            case 1:
                drunkard[0]++;
                break;
            case 2:
                drunkard[1]--;
                break;
            case 3:
                drunkard[1]++;
                break;
            default:
                break;
        }
        if (countdown-- == 0) {
            stopcase = true;
        }
    }
    return worldmap;
}

void world::generate_world() {
    //Procedural world generation here
    worldmap = drunkards_walk();
}

int** world::get_world_map() {
    return worldmap;
}
