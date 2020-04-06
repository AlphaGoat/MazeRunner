#include <SDL2/SDL.h>
#include "../GameMechanics/game_objects.h"
#include "../GameMechanics/stateManagement.h"
#include "camera.h"


void setCameraDisplay(survivor *Player_Character, camera *Camera,
        int **GameGrid, SDL_Texture **textureArray, SDL_Renderer *renderer) {
    /* Sets what camera displays based on the position of the player character, 
     * and what will be rendered on screen */

    /* The (x,y) coordinates of the camera will be based on some offset of the 
     * player character's position, and will depend on if the character is
     * moving or not */
    if (abs(Player_Character->x_velocity) > 0) {
        /* If the player character is within the offsets at the edge of the 
         * camera view, move the character sprite in the camera, rather than
         * the camera itself */
        if      ((Camera->charSpriteCoords->x < (Camera->width - Camera->x_offset)) || 
                (Camera->charSpriteCoords->x > (Camera->x_offset))) 
            Camera->charSpriteCoords->x += Player_Character->x_velocity * TILE_WIDTH;

        /* If the character is at the edge of the offsets, move camera with player
         * character */ 
        else if (((Camera->charSpriteCoords->x >= (Camera->width - Camera->x_offset)) &&
                (Player_Character->x_velocity > 0)) ||
                ((Camera->charSpriteCoords->x <= (Camera->x_offset)) &&
                (Player_Character->x_velocity < 0)))

            Camera->x += Player_Character->x_velocity * TILE_WIDTH;
    }
    
    // If the player character is not moving and the camera is not centered on the 
    // player, slowly drift the camera to be centered to the character's position
    else if ((Player_Character->x_velocity == 0) && 
            ((Camera->x / 2) != Camera->charSpriteCoords->x)) {

            if ((Camera->x / 2) > Camera->charSpriteCoords->x)
                Camera->x -= Camera->camera_drift;

            else 
                Camera->x += Camera->camera_drift;
    }

    if (abs(Player_Character->y_velocity) > 0) {
        /* If the player character is within the offsets at the edge of the 
         * camera view, move the character sprite in the camera, rather than
         * the camera itself */
        if      ((Camera->charSpriteCoords->y < (Camera->width - Camera->y_offset)) || 
                (Camera->charSpriteCoords->y > (Camera->y_offset))) 
            Camera->charSpriteCoords->y += Player_Character->y_velocity * TILE_HEIGHT;

        /* If the character is at the edge of the offsets, move camera with player
         * character */ 
        else if (((Camera->charSpriteCoords->y >= (Camera->width - Camera->y_offset)) &&
                (Player_Character->y_velocity > 0)) ||
                ((Camera->charSpriteCoords->y <= (Camera->y_offset)) &&
                (Player_Character->y_velocity < 0)))

            Camera->y += Player_Character->y_velocity * TILE_HEIGHT;
    }
    
    // If the player character is not moving and the camera is not centered on the 
    // player, slowly drift the camera to be centered to the character's position
    else if ((Player_Character->y_velocity == 0) && 
            ((Camera->y / 2) != Camera->charSpriteCoords->y)) {

            if ((Camera->y / 2) > Camera->charSpriteCoords->y)
                Camera->y -= Camera->camera_drift;

            else 
                Camera->y += Camera->camera_drift;
    }

    

    // Render tiles to map based on the camera's coordinates
    SDL_Rect renderRect; /* Rectangle to keep track of the portion of the camera we are rendering in a given pass */
    renderRect.w = TILE_WIDTH;
    renderRect.h = TILE_HEIGHT;
//    renderRect.x = 0;
//    renderRect.y = Camera->height;

    /* Retrieve grid coords of top left corner of camera view */
    int grid_x = floor(Camera->x / TILE_WIDTH);
    int grid_y = floor(Camera->y / TILE_HEIGHT);

    /* height of camera view in tiles */
    int cam_tile_height = floor(Camera->height / TILE_HEIGHT);

    /* Starting from top left corner of camera screen, render tiles based on map
     * provided by 'GameGrid' */
    for (int i = cam_tile_height; i > 0; i-=1) {
        renderRect.y = i * TILE_HEIGHT;
        for (int j = 0; j < floor(Camera->width / TILE_WIDTH); j+=1) {

            // Update camera rect object to slide render window to next tile
            renderRect.x = j * TILE_WIDTH;

            /* Convert "Camera Grids" into "Game tiles" */
            int tile_row = grid_y - (cam_tile_height - i);
            int tile_col = grid_x + j;

            renderTile(j, i, GameGrid, textureArray, &renderRect, renderer);
            
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

