#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture_app.h"
#include "../GameMechanics/game_objects.h"
#include "../GameMechanics/gameGrid.h"

const int FRAME_DELAY = 100;

void getSpriteCoords(survivor *character, int offset, SDL_Rect *spriteRect, 
        sprite_render_info *sprite_coords) {
    /* fill input sprite sheet rectangle with coordinates of corresponding
     * character sprite 
     * :param spriteRect:      pointer to rectangle providing dims of one character sprite
     *                         in sheet
     * :param spriteSheetDims: pointer to dimensions of sprite sheet
     * */

    /* Get the width and height of one sprite in that sprite sheet */
    int char_width = spriteRect->w;
    int char_height = spriteRect->h;
    /* Sprites for character standing still */
    if (character->x_velocity == 0 && character->y_velocity == 0) {
        switch(character->orientation) {
            case WEST:
                spriteRect->y = sprite_coords->sprite_height + sprite_coords->y_offset;
                spriteRect->x = 0;
                break;
            /* In the case of WEST orientation, these rectangle coords
             * are actually going to apply to the mirror transform of the
             * sprite sheet */
            case EAST:
                spriteRect->y = sprite_coords->sprite_height + sprite_coords->y_offset;
                spriteRect->x =  sprite_coords->sheet_width - sprite_coords->sprite_width;
                break;
            case SOUTH:
                spriteRect->y = sprite_coords->y_offset;
                spriteRect->x = 0;
                break;
            case NORTH:
                spriteRect->y = (2 * sprite_coords->sprite_height) + sprite_coords->y_offset;
                spriteRect->x = 0;
                break;
        }
    }

    else if (character->x_velocity != 0 || character->y_velocity != 0) {
        int frame_num = (SDL_GetTicks() / FRAME_DELAY) % sprite_coords->num_motion_frames;
        switch(character->orientation) {
            case WEST:
                spriteRect->y = sprite_coords->sprite_height + sprite_coords->y_offset;
                spriteRect->x = (1 + frame_num) * sprite_coords->sprite_width;
                break;

            case EAST:
                spriteRect->y = sprite_coords->sprite_height + sprite_coords->y_offset;
                spriteRect->x = sprite_coords->sheet_width - 
                    ((2 + frame_num) * sprite_coords->sprite_width);
                break;

            case SOUTH:
                frame_num -=  2;
                spriteRect->y = sprite_coords->y_offset;
                spriteRect->x = (1 + frame_num) * sprite_coords->sprite_width;
                break;

            case NORTH:
                frame_num -= 2;
                spriteRect->y = (2 * sprite_coords->sprite_height) + sprite_coords->y_offset;
                spriteRect->x = (1 + frame_num) * sprite_coords->sprite_width;
        }
    }
}

SDL_Texture *fetchGeneratorTexture(char *gen_img_path, SDL_Renderer *renderer) {
    /* Fetch texture for generator and perform preprocessing */
    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        /* TODO: write some kind of error handling function here */
        printf("FATAL ERROR. Video subsytems not initialized before texture ops!\n");
        printf("fetchGeneratorTexture failed with critical error\n");
        exit(1);
    }
    SDL_Surface *image = NULL;    
    image = IMG_Load(gen_img_path);
    if (image == NULL) {
        printf("ERROR. Generator image could not be loaded from file\n");
        printf("fetchGeneratorTexture failed with image load.\n");
        exit(1);
    }
    // Set white background as transparent
    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 255, 255, 255)); 
    SDL_Texture *genTexture = NULL;
    if ((genTexture = SDL_CreateTextureFromSurface(renderer, image)) == NULL) {
        printf("ERROR. Could not convert generator image to texture\n");
        printf("fetchGeneratorTexture failed with texture conversion\n");
    }
    SDL_FreeSurface(image); /* Go, my surface. Be free! */
    return genTexture;
}

//SDL_Texture *fetchLightbulbTexture(char *bulb_img_path, SDL_renderer *renderer) {
//
//   if (!SDL_WasInit(SDL_INIT_VIDEO)) { 
//        /* TODO: write some kind of error handling function here */
//        printf("FATAL ERROR. Video subsytems not initialized before texture ops!\n");
//        printf("fetchGeneratorTexture failed with critical error\n");
//        exit(1);
//    }
//    SDL_Surface *image = NULL;    
//    image = IMG_Load(gen_img_path);
//    if (image == NULL) {
//        printf("ERROR. Generator image could not be loaded from file\n");
//        printf("fetchGeneratorTexture failed with image load.\n");
//        exit(1);
//    }
//    // Set white background as transparent
//    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 255, 255, 255)); 
//    SDL_Texture *genTexture = NULL;
//    if ((genTexture = SDL_CreateTextureFromSurface(renderer, image)) == NULL) {
//        printf("ERROR. Could not convert generator image to texture\n");
//        printf("fetchGeneratorTexture failed with texture conversion\n");
//    }
//    SDL_FreeSurface(image); /* Go, my surface. Be free! */
//    return genTexture;
//    
//
//
//
//
