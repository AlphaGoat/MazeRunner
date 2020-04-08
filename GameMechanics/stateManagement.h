#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include "game_objects.h"


enum Tiles {
    grass_tile,
    dirt_tile,
    wall_tile,
    outside_map_tile,
};

int checkForCollision(int x, int y, int **GameGrid);

generator InitGenerators(int x, int y);

int InitMapTerrain(int **GameGrid);

#endif
