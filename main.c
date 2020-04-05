#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Custom modules
#include "GameMechanics/gameGrid.h"
#include "GameMechanics/game_objects.h"
#include "GameMechanics/survivors.h"
#include "GraphicsEngine/texture_app.h"

#include "GraphicsEngine/spriteSheetOps.h"

// Modules that are untested


#define FRAMES_PER_SEC 60

time_t frame_init_time;

int main(int argc, char *argv[]){

//    int height;
//    int width;

    sprite_render_info sailor_moon_sprite_dims;
    sailor_moon_sprite_dims.y_offset = 5;
    sailor_moon_sprite_dims.x_offset = 0;
    sailor_moon_sprite_dims.sheet_height = 900;
    sailor_moon_sprite_dims.sheet_width = 386;
    sailor_moon_sprite_dims.sprite_height = 900 / 20;
    sailor_moon_sprite_dims.sprite_width = 386 / 18;
    sailor_moon_sprite_dims.num_motion_frames = 4;

//    height = width = 100;

//    // Initialize Game Grid
//    int **GameGrid = NULL;
//    int *GameGrid_data = NULL;
//    GameGrid_data = allocateGameGrid(height, width, &GameGrid);
//    if (GameGrid == NULL) {
//        printf("Memory for GameGrid failed to allocate\n");
//        return -1;
//    }

    // Initialize survivors
    survivor sailorMoon;

    sailorMoon.pos_x = 40;
    sailorMoon.pos_y = 40;
    sailorMoon.x_velocity = 0;
    sailorMoon.y_velocity = 0;
    sailorMoon.orientation = SOUTH;
    sailorMoon.health_state = full_health;
    sailorMoon.SpriteRenderInfo = &sailor_moon_sprite_dims;

    // Initialize game window
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *game_window = SDL_CreateWindow("DBD2EB", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 1200, 900, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(game_window, -1, 0);

    SDL_Event input;

    /* Initialize sprite sheet */
    SDL_Texture *spriteSheet = NULL;
    SDL_Texture *flipSpriteSheet = NULL;;
    SDL_Surface *temp = IMG_Load("textures/survivors/sailor_moon_spriters-resource_myaphelion.png");

    /* Perform horinzontal mirror transform on surface to get additional sprite textures */
    SDL_Surface *temp_flipped = NULL;
    temp_flipped =  horizMirrReflectPixelMap(temp);

    /* Obtain textures from sprite sheet and mirror reflection of sprite sheet
     * to get full moveset */
    spriteSheet = SDL_CreateTextureFromSurface(renderer, temp);
    flipSpriteSheet = SDL_CreateTextureFromSurface(renderer, temp_flipped);

    SDL_FreeSurface(temp);
    SDL_FreeSurface(temp_flipped);

    /* window rect defines placement of character sprite on game window */
    SDL_Rect spriteWindowCoords;
    spriteWindowCoords.x = 600;
    spriteWindowCoords.y = 450;
    spriteWindowCoords.w = 20;
    spriteWindowCoords.h = 30;

    /* sprite rect gives coordinates of sprite on sprite sheet */
    SDL_Rect rcSheetSprite;
    rcSheetSprite.x = 0;
    rcSheetSprite.y = 5;

    /* Get the actual dimensions of the sprite sheet */
    SDL_QueryTexture(spriteSheet, NULL, NULL, &rcSheetSprite.w, &rcSheetSprite.h);

    /* Determine and record the height and width of individual sprites in sheet */
    rcSheetSprite.w = rcSheetSprite.w / 18;
    rcSheetSprite.h = rcSheetSprite.h / 20; 


    /* Initate game loop */
    int quit = 0;
    while (!quit) {

        frame_init_time = SDL_GetTicks();
        while (SDL_PollEvent(&input) > 0) {
            if (input.type == SDL_QUIT)
                quit = 1;
            /* Use wasd key down events to move character */
            else if (input.type == SDL_KEYDOWN) {
                switch(input.key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_a:
                        sailorMoon.x_velocity = -1;
                        sailorMoon.orientation = WEST;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        sailorMoon.x_velocity = 1;
                        sailorMoon.orientation = EAST;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        sailorMoon.y_velocity = 1;
                        sailorMoon.orientation = SOUTH;
                        break;
                    case SDLK_UP:
                    case SDLK_w:
                        sailorMoon.y_velocity = -1;
                        sailorMoon.orientation = NORTH;
                        break;
                    default:
                        break;
                }
            }
            /* If a key up state is registered, we need to
             * zero out the velocity (but only zero it out if the
             * velocity corresponds with the correct keydown event.
             * Otherwise we might be zeroing out the value for a left up
             * event, even if the right key is still pressed down */
            else if (input.type == SDL_KEYUP) {
                switch(input.key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_a:
                        if (sailorMoon.x_velocity < 0)
                            sailorMoon.x_velocity = 0;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (sailorMoon.x_velocity > 0)
                            sailorMoon.x_velocity = 0;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        if (sailorMoon.y_velocity > 0)
                            sailorMoon.y_velocity = 0;
                        break;
                    case SDLK_UP:
                    case SDLK_w:
                        if (sailorMoon.y_velocity < 0)
                            sailorMoon.y_velocity = 0;
                        break;
                    default:
                        break;

//                // If both the x_velocity and the y_velocity of the
//                // survivor are 0, start idle timer
//                if (sailorMoon.y_velocity == 0) && (sailorMoon.x_velocity == 0)
//                    sailorMoon.idle_time_init = SDL_GetTicks();
                        
                }
            }
        }



        /* Determine what set of sprite animations to use */
        /* When sedentary */
        getSpriteCoords(&sailorMoon, 5, &rcSheetSprite, sailorMoon.SpriteRenderInfo);

        /* Update sailor moon's position */
        sailorMoon.pos_x += sailorMoon.x_velocity;
        sailorMoon.pos_y += sailorMoon.y_velocity;

//        SDL_Rect *spriteWindowCoords;
        moveSurvivorInWindow(sailorMoon, &spriteWindowCoords);

//        /* Update sailor moon's position in the game window */
//        spriteWindowCoords.x = sailorMoon.pos_x;
//        spriteWindowCoords.y = sailorMoon.pos_y;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        //Copy texture on to window using
        //renderer, sprite rectangle, and window rectangle
        if (sailorMoon.orientation == EAST) 
            /* If survivor is facing right, then we'll need to use flipped sprite sheet */
            SDL_RenderCopy(renderer, flipSpriteSheet, &rcSheetSprite, &spriteWindowCoords);
        else
            SDL_RenderCopy(renderer, spriteSheet, &rcSheetSprite, &spriteWindowCoords);
        SDL_RenderPresent(renderer);

        // Do nothing until the timer for 1 frame expires
//        sleep(1.0/FRAMES_PER_SEC);
//        while(SDL_GetTicks() - frame_init_time < 1000/FRAMES_PER_SEC);
        SDL_Delay(abs(1000/FRAMES_PER_SEC - (SDL_GetTicks() - frame_init_time)));
//        if ((SDL_GetTicks() - frame_init_time) < 1000/FRAMES_PER_SEC)
//            sleep(fabs((1.0/FRAMES_PER_SEC) - (SDL_GetTicks() - frame_init_time)/1000));
//        sleep(1.0/FRAMES_PER_SEC - (SDL_GetTicks() - frame_init_time)/1000);
    }

    /* Destroy SDL objects that were created */
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(game_window);
    IMG_Quit();
    SDL_Quit();
//    freeGameGridMem(GameGrid_data, &GameGrid);

    return 1;
}
