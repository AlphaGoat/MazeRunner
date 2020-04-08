#include <SDL2/SDL.h>
#include "game_objects.h"
#include "gameGrid.h"
#include "stateManagement.h"
#include "../defs.h"

//survivor *InitSurvivor(int start_posx, int start_posy,
//
//    /* Survivor's position on GameGrid */
//    sailorMoon.pos_x = 400;
//    sailorMoon.pos_y = 400;
//    sailorMoon.x_velocity = 0;
//    sailorMoon.y_velocity = 0;
//    sailorMoon.orientation = SOUTH;
//    sailorMoon.health_state = full_health;
//    sailorMoon.SpriteRenderInfo.y_offset = 5;
//    sailorMoon.SpriteRenderInfo.x_offset = 0;
//    sailorMoon.SpriteRenderInfo.sheet_height = 900;
//    sailorMoon.SpriteRenderInfo.sheet_width = 386;
//    sailorMoon.SpriteRenderInfo.sprite_height = 900 / 20;
//    sailorMoon.SpriteRenderInfo.sprite_width = 386 / 18;
//    sailorMoon.SpriteRenderInfo.num_motion_frames = 4;
//    sailorMoon.SpriteRenderInfo.xmotion_flag = 0;
//    sailorMoon.SpriteRenderInfo.ymotion_flag = 0;


void getCharSpriteChoords(survivor *character, SDL_Rect *spriteRect) {
    /* fill input sprite sheet rectangle with coordinates of corresponding
     * character sprite */
    
    
    if ((*character).x_velocity == 0 && (*character).y_velocity == 0) {
        switch((*character).orientation) {
            case EAST:
                (*spriteRect).y = (2 * (*spriteRect).h) + 5;
                (*spriteRect).x = 0;
                break;
            case WEST:
                (*spriteRect).y = (2 * (*spriteRect).h) + 5;
                (*spriteRect).x = 0;
                break;
            case SOUTH:
                break;
            case NORTH:
                break;
        }
    }
}

int checkForTerrainCollision_x(int pos_x, int pos_y, int x_velocity, int y_velocity, 
        int char_width, int char_height, int orientation, int **GameGrid) {
    /* Checks position to see if there is impassible terrain in way
     * of player */

    /* Might add a height offset variable to allow a character to "collide"
     * a little bit with tile */

    int new_posx = pos_x + x_velocity;
    int new_posy = pos_y + y_velocity;

    /* Check if position puts player out of bounds */
    if ((new_posy > GAME_HEIGHT * TILE_HEIGHT) || (new_posy - char_height < 0) || (new_posx + char_width > GAME_WIDTH * TILE_WIDTH) || (new_posx < 0))
        return 1;


    int left_gridx = new_posx / TILE_WIDTH;
    int right_gridx = (new_posx + char_width)/TILE_WIDTH;

    int top_gridy = new_posy/TILE_HEIGHT;
    int bottom_gridy = (new_posy - char_height)/TILE_HEIGHT;

    /* If the character is fully contained within one grid, hopefully
     * the detection collision algorithm already picked up that they
     * are okay to move there... */
    if ((top_gridy == bottom_gridy) && (left_gridx == right_gridx))
        return 0;

    /* Flags of what tiles to check */
    int tiles[3];
    int tile_count = 0;

    if (orientation == WEST) {

        /* Check if character is touching tile to left */
        if (new_posx > (left_gridx * TILE_WIDTH))
            return 0;

        /* Check what y tiles we need to check */
        else {
            int col = new_posx / TILE_WIDTH;
            if ((new_posy / TILE_HEIGHT) == (new_posy - char_height)/TILE_HEIGHT) {
                /* only need to check tile to immediate left */
                int row = new_posy / TILE_HEIGHT;
                tiles[0] = *((*GameGrid) + (GAME_HEIGHT * row) + col);
                tile_count = 1;
            }

            /* if char height is greater than a single tile, then we need to
             * check three tiles */
            else if ((char_height > TILE_HEIGHT) && 
                    ((new_posy / TILE_HEIGHT) - 
                    ((new_posy - char_height)/ TILE_HEIGHT) > 1)) {
                int row_above = new_posy / TILE_HEIGHT;
                int row_below = (new_posy - char_height) / TILE_HEIGHT;
                int row_adjacent = row_above - 1;
                tiles[0] = *((*GameGrid) + (GAME_HEIGHT * row_above) + col);
                tiles[1] = *((*GameGrid) + (GAME_HEIGHT * row_below) + col);
                tiles[2] = *((*GameGrid) + (GAME_HEIGHT * row_adjacent) + col);
                tile_count = 3;
            }

            else {
                int row_above = new_posy / TILE_HEIGHT;
                int row_below = (new_posy - char_height) / TILE_HEIGHT;
                tiles[0] = *((*GameGrid) + (GAME_HEIGHT * row_above) + col);
                tiles[1] = *((*GameGrid) + (GAME_HEIGHT * row_below) + col);
                tile_count = 2;
            }
        }
    }

    else if (orientation == EAST) {

        /* Check if character is touching tile to right */
        if ((new_posx + char_width) > (right_gridx * TILE_WIDTH))
            return 0;

        /* Check what y tiles we need to check */
        else {
            int col = (new_posx + char_width) / TILE_WIDTH;
            if ((new_posy / TILE_HEIGHT) == (new_posy - char_height)/TILE_HEIGHT) {
                /* only need to check tile to immediate left */
                int row = new_posy / TILE_HEIGHT;
                tiles[0] = *((*GameGrid) + (GAME_HEIGHT * row) + col);
                tile_count = 1;
            }

            /* if char height is greater than a single tile, then we need to
             * check three tiles */
            else if ((char_height > TILE_HEIGHT) && 
                    ((new_posy / TILE_HEIGHT) - 
                    ((new_posy - char_height)/ TILE_HEIGHT) > 1)) {
                int row_above = new_posy / TILE_HEIGHT;
                int row_below = (new_posy - char_height) / TILE_HEIGHT;
                int row_adjacent = row_above - 1;
                tiles[0] = *((*GameGrid) + (GAME_HEIGHT * row_above) + col);
                tiles[1] = *((*GameGrid) + (GAME_HEIGHT * row_below) + col);
                tiles[2] = *((*GameGrid) + (GAME_HEIGHT * row_adjacent) + col);
                tile_count = 3;
            }

            else {
                int row_above = new_posy / TILE_HEIGHT;
                int row_below = (new_posy - char_height) / TILE_HEIGHT;
                tiles[0] = *((*GameGrid) + (GAME_HEIGHT * row_above) + col);
                tiles[1] = *((*GameGrid) + (GAME_HEIGHT * row_below) + col);
                tile_count = 2;
            }
        }
    }

    /* See if the tiles the character is in contact with are collidable */
    for (int i = 0; i < tile_count; i++) {
        int tile = tiles[i];
        switch (tile) {
            case wall_tile:
                return 1;
            default:
                return 0;
        }
    }

    return 0;
}
//void *moveSurvivorInWindow (survivor player_char,
//        SDL_Rect *spriteWindowRect) {
//
//        /* If still within movement box, Update sailor moon's 
//         * position in the window */
//        if ((spriteWindowRect->x <= 700) && (spriteWindowRect->x >= 500)) {
//            spriteWindowRect->x += player_char.x_velocity * TILE_WIDTH;
//        }
//
//        else if ((spriteWindowRect->x > 700)) {
//            if (player_char.x_velocity < 0)
//                spriteWindowRect->x += player_char.x_velocity * TILE_HEIGHT;
//        }
//
//        else if (spriteWindowRect->x < 500) {
//            if (player_char.x_velocity > 0) 
//                spriteWindowRect->x += player_char.x_velocity;
//        }
//
//        /* Update y position */
//        if ((spriteWindowRect->y <= 550) && (spriteWindowRect->y >= 350)) {
//            spriteWindowRect->y += player_char.y_velocity;
//        }
//
//        else if ((spriteWindowRect->y > 550)) {
//            if (player_char.y_velocity < 0)
//                spriteWindowRect->y += player_char.y_velocity;
//        }
//
//        else if (spriteWindowRect->y < 350) {
//            if (player_char.y_velocity > 0) 
//                spriteWindowRect->y += player_char.y_velocity;
//        }
//        spriteWindowRect->w = 20;
//        spriteWindowRect->h = 30;
//
//}
