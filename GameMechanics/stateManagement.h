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

generator * InitGenerators(int num_generators, int map_height, int map_width,
        int bound_gen_radius, int **GameGrid);

int InitMapTerrain(int **GameGrid);

#endif
