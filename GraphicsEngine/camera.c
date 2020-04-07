#include <SDL2/SDL.h>
#include "../GameMechanics/game_objects.h"
#include "../GameMechanics/stateManagement.h"
#include "camera.h"
#include "../defs.h"

void setCameraDisplay(survivor **Player_Character, camera **Camera,
        int **GameGrid, SDL_Texture **textureArray, SDL_Renderer *renderer) {
    /* Sets what camera displays based on the position of the player character, 
     * and what will be rendered on screen */

    /* Calculate appropiate camera coordinates */
    calculateCameraCoords(Camera, Player_Character);

    // Render tiles to map based on the camera's coordinates
    SDL_Rect renderRect; /* Rectangle to keep track of the grid we are rendering */
    renderRect.w = TILE_WIDTH;
    renderRect.h = TILE_HEIGHT;

    /* Retrieve grid coords of top left corner of camera view */
    int grid_x = floor((*Camera)->x / TILE_WIDTH);
    int grid_y = floor((*Camera)->y / TILE_HEIGHT);

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

void calculateCameraCoords(camera **Camera, survivor **PlayerCharacter) {

    // Calculate offsets between internally tracked position of character in the world 
    // and the coordinates of the character in the view port as well as the coordinates
    // of the camera itself
    int camera_height = (*Camera)->height;
    int camera_width = (*Camera)->width;
    int cam_x = (*Camera)->x;
    int cam_y = (*Camera)->y;
    int cam_xoffset = (*Camera)->x_offset;
    int cam_yoffset = (*Camera)->y_offset;
    int camera_drift = (*Camera)->camera_drift;

    int char_x = (*Camera)->spriteCoords.x;
    int char_y = (*Camera)->spriteCoords.y;

    int x_velocity = (*PlayerCharacter)->x_velocity;
    int y_velocity = (*PlayerCharacter)->y_velocity;

    if (x_velocity > 0 && ((camera_width - char_x) > cam_xoffset)) 
       /* If the position of the character in the viewport is still outside
        * the bounds of the edge offset, move the character in the viewport
        * but not the camera itself */
        (*Camera)->spriteCoords.x += x_velocity;
    
//    else if (x_velocity < 0 && (char_x <= cam_xoffset))
//        (*Camera)->spriteCoords.x += x_velocity;
    else if (x_velocity < 0 && (char_x > cam_xoffset))
        (*Camera)->spriteCoords.x += x_velocity;

    else if (abs(x_velocity) > 0 && 
            ((camera_width - char_x <= cam_xoffset) ||
             (char_x <= cam_xoffset))) {
        /* If the position of the character in the viewporst is inside
         * the bounds of the edge offset, move the camera coordinates,
         * but not the coordinates of the character in the view port */
        (*Camera)->x += x_velocity;
    }

    else if ((x_velocity == 0) && (char_x != camera_height / 2)) {

        if (abs(char_x - (camera_width / 2)) < camera_drift) {
            /* If the difference between the char's position and the centroid
             * is less than the camera's drift, snap the character to the centroid */
            (*Camera)->spriteCoords.x = camera_width / 2;
        }

        else if (char_x - (camera_width / 2) < 0) {
            (*Camera)->x -= camera_drift;
            (*Camera)->spriteCoords.x += camera_drift;
        }
        
        else {
            (*Camera)->x += camera_drift;
            (*Camera)->spriteCoords.x -= camera_drift;
        }

    }

    if (y_velocity > 0 && (camera_height - char_y > cam_yoffset)) {
       /* If the position of the character in the viewport is still outside
        * the bounds of the edge offset, move the character in the viewport
        * but not the camera itself */
        (*Camera)->spriteCoords.y += y_velocity;

    }

    else if (y_velocity < 0 && (char_y > cam_yoffset))
        (*Camera)->spriteCoords.y += y_velocity;

    else if (abs(y_velocity) > 0 && ((abs(camera_height - char_y) <= cam_yoffset) ||
                (char_y <= cam_yoffset))) {
        /* If the position of the character in the viewporst is inside
         * the bounds of the edge offset, move the camera coordinates,
         * but not the coordinates of the character in the view port */
        (*Camera)->y += y_velocity;

    }

    else if ((y_velocity == 0) && (char_y != camera_height / 2)) {

        if (abs(char_y - (camera_height / 2)) < camera_drift) {
            /* If the difference between the char's position and the centroid
             * is less than the camera's drift, snap the character to the centroid */
            char_y = camera_height / 2;
        }

        else if (char_y - (camera_height / 2) < 0) {
            (*Camera)->y -= camera_drift;
            (*Camera)->spriteCoords.y += camera_drift;
        }
        
        else  {
            (*Camera)->y += camera_drift;
            (*Camera)->spriteCoords.y -= camera_drift;
        }

    }

    (*PlayerCharacter)->pos_x = cam_x + char_x;
    (*PlayerCharacter)->pos_y = cam_y - char_y;
}



             






