#ifndef GAMEGRID_H
#define GAMEGRID_H


enum Orientation {
    NORTH,
    SOUTH,
    EAST,
    WEST,
};

// Custom functions
int *allocateGameGrid(int height, int width, int ***GameGrid); /* Allocates memory for game grid of
                                                               * given height x width */

void freeGameGridMem(int *GameGrid_data, int ***GameGrid); /* Free memory allocated for game grid */

#endif
