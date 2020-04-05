/* Functions to initialize game grid and populate it with necessary
 * structures, like generators, hooks, exits, etc. */
#include <stdlib.h>
#include <stdio.h>

//void allocateGameGrid(int height, int width, int ***GameGrid_ptr) {
//    *GameGrid_ptr = (int**) malloc(height * sizeof(int*));
//    for (int i = 0; i < height; i++)
//        *GameGrid_ptr[i] = (int*) malloc(width * sizeof(int));
//}
//
//void freeGameGridMemory(int height, int ***GameGrid_ptr){
//    for (int i = 0; i < height; i++)
//        free((*GameGrid_ptr)[i]);
//    free(*GameGrid_ptr);
//    *GameGrid_ptr = NULL;
//}

int* allocateGameGrid(int height, int width, int ***GameGrid_ptr) {
    /* Allocating second-level memory with one call to malloc, instead
     * of multiple */
    *GameGrid_ptr = (int**)malloc(height * sizeof(int*));
    int *GameGrid_data = malloc(height * width * sizeof(int));
    for (int i = 0; i < height; i++)
        (*GameGrid_ptr)[i] = GameGrid_data + i * width;
    return GameGrid_data;
}

void freeGameGridMem(int *GameGrid_data, int ***GameGrid_ptr) {
    free(GameGrid_data);
    free(*GameGrid_ptr);
}
