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
        if (Player_Character->pos_x < (Camera->width - Camera->x_offset) ||
                Player_Character->pos_x > (Camera->x_offset))
            Camera->x += Player_Character->x_velocity;
    }
    
    // If the player character is not moving and the camera is not centered on the 
    // player, slowly drift the camera to be centered to the character's position
    else if ((Player_Character->x_velocity == 0) && 
            (Camera->x != Player_Character->pos_x)) {

            if (Camera->x > Player_Character->pos_x)
                Camera->x += Camera->camera_drift;
    }

    
    if (abs(Player_Character->y_velocity) > 0) {
        if (Player_Character->pos_y < (Camera->width - Camera->y_offset) ||
                Player_Character->pos_y > (Camera->y_offset))
            Camera->y += Player_Character->y_velocity;
    }


    else if ((Player_Character->y_velocity == 0) && 
            (Camera->y != Player_Character->pos_y)) {

            if (Camera->y > Player_Character->pos_y)
                Camera->y += Camera->camera_drift;
    }

    // Render tiles to map based on the camera's coordinates
    SDL_Rect renderRect; /* Rectangle to keep track of the portion of the camera we are rendering in a given pass */
    renderRect.w = TILE_WIDTH;
    renderRect.h = TILE_HEIGHT;
    renderRect.x = 0;
    renderRect.y = Camera->height;
    for (int row = floor((Camera->y - Camera->height) / TILE_HEIGHT); row < floor(Camera->y / TILE_HEIGHT); row++) {
        for (int col = floor(Camera->x / TILE_WIDTH); col < floor((Camera->width - Camera->x) / TILE_WIDTH); col++) {
            renderTile(col, row, GameGrid, textureArray, &renderRect, renderer);
            
            // Update camera rect object to slide render window to next tile
            renderRect.x += TILE_WIDTH;
           
            // On final x in row, reset x val and decrement y by tile height
            if (renderRect.x == Camera->width) {
                renderRect.x = 0;
                renderRect.y -= TILE_HEIGHT;
            }

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
    // Fetch tile type from GameGrid
    int tile_type = GameGrid[row][col];

    // Fetch texture information for tile
    SDL_Texture *texture = textureArray[tile_type]; 

    // Render texture on game window
    SDL_RenderCopy(renderer, texture, NULL, renderRect);
}
