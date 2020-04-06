#include <SDL2/SDL.h>
#include "../GameMechanics/game_objects.h"
#include "../GameMechanics/stateManagement.h"
#include "camera.h"
#include "../defs.h"


void setCameraDisplay(survivor *Player_Character, camera **Camera,
        int **GameGrid, SDL_Texture **textureArray, SDL_Renderer *renderer) {
    /* Sets what camera displays based on the position of the player character, 
     * and what will be rendered on screen */

    /* The (x,y) coordinates of the camera will be based on some offset of the 
     * player character's position, and will depend on if the character is
     * moving or not */
    int x_offset = (*Camera)->x_offset;
    int y_offset = (*Camera)->y_offset;

    /* Dimensions of camera */
    int cam_width = (*Camera)->width;
    int cam_height = (*Camera)->height;

    /* position of character in camera view port */
    int char_posx = (*Camera)->charSpriteCoords->x;
    int char_posy = (*Camera)->charSpriteCoords->y;

    /* position of the upper left corner of camera, in
     * pixel coordinates */
    int cam_posx = (*Camera)->x;
    int cam_posy = (*Camera)->y;

    /* Velocity of player character */
    int x_velocity = Player_Character->x_velocity;
    int y_velocity = Player_Character->y_velocity;

    /* Position of character in game world */
    int world_posx = Player_Character->pos_x;
    int world_posy = Player_Character->pos_y;

    if (abs(x_velocity) > 0) {
        /* If the character is at the edge of the offsets, move camera with player
         * character */ 
        if (((char_posx >= ((*Camera)->width - x_offset)) &&
                (x_velocity > 0)) ||
                ((char_posx <= x_offset) &&
                (x_velocity < 0)))
            (*Camera)->x += x_velocity; // * TILE_WIDTH;
        else
            (*Camera)->charSpriteCoords->x += x_velocity; // * TILE_WIDTH;
    }
    // If the player character is not moving and the camera is not centered on the 
    // player, slowly drift the camera to be centered to the character's position
    else if ((x_velocity == 0) && (char_posx > ((*Camera)->width / 2))) {
        /* If the distance left to cover to center the camera is less than the
         * drift value, snap the camera to the centered position */
        /* Drift camera right */
        if ((char_posx - ((*Camera)->width / 2)) < ((*Camera)->camera_drift)) {
            (*Camera)->x = ((*Camera)->width / 2) + world_posx;
            char_posx = (*Camera)->x - world_posx;
        }
        /* Drift camera right */
        else {
            (*Camera)->x += (*Camera)->camera_drift;
            (*Camera)->charSpriteCoords->x -= (*Camera)->camera_drift;
        }
    }

    else if ((x_velocity == 0) && (char_posx < ((*Camera)->width / 2))) {
        /* If the distance left to cover to center the camera is less than the
         * drift value, snap the camera to the centered position */
        /* Drift camera right */
        if ((((*Camera)->width / 2) - char_posx) < ((*Camera)->camera_drift)) {
            (*Camera)->x = ((*Camera)->width / 2) + world_posx;
            char_posx = (*Camera)->x - world_posx;
        }
        /* Drift camera left */
        (*Camera)->x -=(*Camera)->camera_drift;
        (*Camera)->charSpriteCoords->x += (*Camera)->camera_drift;
    }

//    // Otherwise, if player character is still and centered, perform a 
//    // sanity check to see if camera reflects position in world, and adjust accordingly
//    // if not
//    else

    if (abs(y_velocity) > 0) {

        /* If the character is at the edge of the offsets, move camera with player
         * character */ 
        if (((char_posy >= ((*Camera)->height - y_offset)) &&
                (y_velocity > 0)) ||
                ((char_posy <= y_offset) &&
                 (y_velocity < 0)))

//           (*Camera)->y = (world_posy * TILE_HEIGHT) + (cam_height / 2);
            (*Camera)->y += y_velocity; // * TILE_HEIGHT
        else
           (*Camera)->charSpriteCoords->y += y_velocity; // * TILE_WIDTH;
    }
    
    else if ((y_velocity == 0) && (char_posy > ((*Camera)->height / 2))) {
        /* If the distance left to cover to center the camera is less than the
         * drift value, snap the camera to the centered position */
        /* Drift camera right */
        if ((char_posy - ((*Camera)->height / 2)) < ((*Camera)->camera_drift)) {
            (*Camera)->y = ((*Camera)->height / 2) + world_posy;
            char_posy = (*Camera)->y - world_posy;
        }
        else {
        (*Camera)->y += (*Camera)->camera_drift;
        (*Camera)->charSpriteCoords->y -= (*Camera)->camera_drift;
        }
    }

    else if ((y_velocity == 0) && (char_posy < ((*Camera)->height / 2))) {
        /* If the distance left to cover to center the camera is less than the
         * drift value, snap the camera to the centered position */
        /* Drift camera right */
        if ((((*Camera)->height / 2) - char_posy) < ((*Camera)->camera_drift)) {
            (*Camera)->y = ((*Camera)->height / 2) + world_posy;
            char_posy = (*Camera)->y - world_posy;
        }
        /* Drift camera left */
        else {
            (*Camera)->y -=(*Camera)->camera_drift;
            (*Camera)->charSpriteCoords->y += (*Camera)->camera_drift;
        }
    }

    // Render tiles to map based on the camera's coordinates
    SDL_Rect renderRect; /* Rectangle to keep track of the portion of the camera we are rendering in a given pass */
    renderRect.w = TILE_WIDTH;
    renderRect.h = TILE_HEIGHT;
//    renderRect.x = 0;
//    renderRect.y =(*Camera)->height;

    /* Retrieve grid coords of top left corner of camera view */
    int grid_x = floor((*Camera)->x / TILE_WIDTH);
    int grid_y = floor((*Camera)->y / TILE_HEIGHT);

//    if ((grid_x != 120) && (grid_y != 90)) {
        //printf("grid_x: %d\n", grid_x);
        //printf("grid_y: %d\n", grid_y);
 //   }

    /* height of camera view in tiles */
    int cam_tile_height = floor((*Camera)->height / TILE_HEIGHT);

    /* Starting from top left corner of camera screen, render tiles based on map
     * provided by 'GameGrid' */
    for (int i = cam_tile_height; i > 0; i-=1) {
        renderRect.y = i * TILE_HEIGHT;
        for (int j = 0; j < floor((*Camera)->width / TILE_WIDTH); j+=1) {

            // Update camera rect object to slide render window to next tile
            renderRect.x = j * TILE_WIDTH;

            /* Convert "Camera Grids" into "Game tiles" */
            int tile_row = grid_y - (cam_tile_height - i);
            int tile_col = grid_x + j;
            renderTile(tile_col, tile_row, GameGrid, textureArray, &renderRect, renderer);
            
        }
    }
}

void renderTile(int col, int row, int **GameGrid, 
        SDL_Texture **textureArray, SDL_Rect *renderRect, SDL_Renderer *renderer) {
    /* Renders a single tile on the game window
     * :param col:          column coordinate on game grid of tile to render
     * :param row:          row coordinate on game grid of tile to render
     * :param cameraRect:   coordinates of where tile will be rendered on view window
     * :param textureArray: pointer to an array of pointers which each point to a 
     *                      texture object for the corresponding texture type
     */
    int tile_type; 
    /* If the tile is outside the game map, render a black space tile */
    if ((row >= GAME_HEIGHT) || (row < 0) || (col >= GAME_WIDTH) || (col < 0))
        tile_type = outside_map_tile;

    /* Otherwise, fetch tile type from Game Grid and render that */
    else
        tile_type = *(*(GameGrid) + (row * GAME_HEIGHT) + col);

    // Fetch texture information for tile
    SDL_Texture *texture = *(textureArray + tile_type); 

    // Render texture on game window
    SDL_RenderCopy(renderer, texture, NULL, renderRect);
}

