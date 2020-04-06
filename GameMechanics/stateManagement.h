#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include "game_objects.h"

/* Some state constants */
static const int NUM_SURVIVORS = 4;
static const int NUM_GENS_TO_COMPLETE = 5;

/* Dimensions of Game World, in tiles */
static const int GAME_HEIGHT = 500;
static const int GAME_WIDTH = 500;

/* Dimensions of individual tiles, in pixels */
static const int TILE_HEIGHT = 10;
static const int TILE_WIDTH = 10;


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
