#include <SDL2/SDL.h>
#include "game_objects.h"
#include "gameGrid.h"

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

void *moveSurvivorInWindow (survivor player_char,
        SDL_Rect *spriteWindowRect) {

        /* If still within movement box, Update sailor moon's 
         * position in the window */
        if ((spriteWindowRect->x <= 700) && (spriteWindowRect->x >= 500)) {
            spriteWindowRect->x += player_char.x_velocity;
        }

        else if ((spriteWindowRect->x > 700)) {
            if (player_char.x_velocity < 0)
                spriteWindowRect->x += player_char.x_velocity;
        }

        else if (spriteWindowRect->x < 500) {
            if (player_char.x_velocity > 0) 
                spriteWindowRect->x += player_char.x_velocity;
        }

        /* Update y position */
        if ((spriteWindowRect->y <= 550) && (spriteWindowRect->y >= 350)) {
            spriteWindowRect->y += player_char.y_velocity;
        }

        else if ((spriteWindowRect->y > 550)) {
            if (player_char.y_velocity < 0)
                spriteWindowRect->y += player_char.y_velocity;
        }

        else if (spriteWindowRect->y < 350) {
            if (player_char.y_velocity > 0) 
                spriteWindowRect->y += player_char.y_velocity;
        }
        spriteWindowRect->w = 20;
        spriteWindowRect->h = 30;

}
