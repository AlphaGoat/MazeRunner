/* Header file specifically to provide specific coordinates for
 * character textures in sprite sheets */
#ifndef TEXTUREAPP_H
#define TEXTUREAPP_H

#include <SDL2/SDL.h>

typedef struct SpriteRenderInfo {
    int y_offset; /* Defines offset from top of sprite sheet */
    int x_offset;
    int sprite_height; /* pixel height of one character sprite */
    int sprite_width;  /* pixel width of one character sprite */
    int sheet_height; /* pixel height of entire sprite sheet */
    int sheet_width; /* pixel width of entire sprite sheet */

    /* Multiplication Factors */
    // The following are arrays of multiplicative factors to apply to sprite height
    // and sprite width to get to desired sprite in sheet ((mul_height, mul_width))
    int still_north;
    int still_south;
    int still_east;
    int still_west;

    /* Number of movement frames per char orientation in spreadsheet */
    int num_motion_frames;
} sprite_render_info;

SDL_Texture * InitializeGreenTexture(SDL_Renderer *renderer);

int initializeTileTextures(SDL_Texture **texture_array, SDL_Renderer *renderer);

#endif


