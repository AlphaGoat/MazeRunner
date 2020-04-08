#include <stdlib.h>
#include <math.h>

/* Custom Modules */
#include "survivors.h"
#include "gameGrid.h"
#include "stateManagement.h"
#include "../defs.h"



// Structure that manages the world state of the 
// game (positions of all objects and player in the
// game world, victory conditions, etc).
typedef struct GameState {
    /* Pointer to array of all survivor objects */
    survivor *survivor_array_ptr;

    /* pointer to killer object */
    killer *killer_ptr;

    generator *generator_ptr; /* Pointer to array of all generator objects */

    /* counter for number of generators completed */
    int num_gens_completed;

    int victory_flag; /* 0 if victory conditions not met, 1 if survivor
                       * victory, 2 if killer victory */

} game_state;


game_state * InitGameState(int map_height, int map_width, int num_generators) {

    /* initialize game grid */
    int **GameGrid = NULL;
    int *GameGrid_data = NULL;
    GameGrid_data = allocateGameGrid(map_height, map_width, &GameGrid);
    if (GameGrid == NULL) {
        printf("Memory for GameGrid failed to allocate\n");
        printf("FATAL ERROR. Exiting\n");
        exit(-1);
    }

    // Initialize game state structure
    game_state CurrentGameState;
    game_state *game_state_ptr;

    //Initialize map terrain

    /* Recieve pointer to initialized generators */
    generator *gen_array_ptr;
    int bound_gen_radius = 50;
//    gen_array_ptr = InitGenerators(num_generators, 
//            bound_gen_radius, GameGrid);
    
    return game_state_ptr;
};

/* Function to initialize terrain of map */
int InitMapTerrain(int **GameGrid) {
    for (int row = 0; row < GAME_HEIGHT; row++) {
        for (int col = 0; col < GAME_WIDTH; col++) {
            int tile_choice = rand() % 3;
            switch(tile_choice) {
//                case 0:
//                    *((*GameGrid) + (row * GAME_HEIGHT) + col) = grass_tile;
//                    break;
//                case 1:
//                    *((*GameGrid) + (row * GAME_HEIGHT) + col) = dirt_tile;
//                    break;
//                case 2:
//                    *((*GameGrid) + (row * GAME_HEIGHT) + col) = wall_tile;
//                    break;
//                default:
//                    *((*GameGrid) + (row * GAME_HEIGHT) + col) = grass_tile;
//                    break;
                    case 0:
                    case 1:
                    case 3:
                        *((*GameGrid) + (row * GAME_HEIGHT) + col) = grass_tile;
                        break;

            }
        }
    }
    return 1;
}

/* Function to initialize and randomly place generators in world map */
generator InitGenerators(int x, int y) {

    generator curr_generator;
    curr_generator.pos_x = x;
    curr_generator.pos_y = y;
    curr_generator.fixed = 0;
    curr_generator.completion_bar = 100;

//    render_info genRenderInfo;
//    genRenderInfo.x = x;
//    genRenderInfo.y = y;
//    genRenderInfo.sheet_width = 582;
//    genRenderInfo.sheet_height = 612;
//    genRenderInfo.width = 582 / 10;
//    genRenderInfo.height = 612 / 10;
//    genRenderInfo.factor_x = 10;
//    genRenderInfo.factor_y = 10;
//    genRenderInfo.texture_flag = enumGeneratorTexture; 
        
    return curr_generator;
}

survivor * InitSurvivors(int num_survivors, int map_height, int map_width);

int checkForCollision(int x, int y, int **GameGrid) {
    /* Checks game grid to see if the tile at position (x,y)
     * collides with game objects */
    int tile_type = *(*(GameGrid) + y * GAME_HEIGHT + x);

    switch(tile_type) {
        case grass_tile:
        case dirt_tile:
            return 0;
        case wall_tile:
        case outside_map_tile:
            return 1;
        default:
            return 0;
    }
}

    
                    

        



                






