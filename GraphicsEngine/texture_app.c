#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture_app.h"
#include "../defs.h"
#include "../GameMechanics/game_objects.h"
#include "../GameMechanics/gameGrid.h"
#include "../GameMechanics/stateManagement.h"


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
int initializeTileTextures(SDL_Texture **texture_array,  
        SDL_Renderer *renderer) {
    /* Initialize map textures */
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    /* GRASS */
    SDL_Surface *grass_surface = NULL;
    SDL_Texture *grass_texture = NULL;
    grass_surface = IMG_Load("./textures/map/grassdirt-small.png");
    if (grass_surface == NULL) 
        printf("grass_texture could not be loaded\n");
    grass_texture = SDL_CreateTextureFromSurface(renderer, grass_surface);
    *(texture_array) = grass_texture;
    SDL_FreeSurface(grass_surface);

    /* DIRT */
    SDL_Surface *dirt_surface = NULL;
    SDL_Texture *dirt_texture = NULL;
    dirt_surface = IMG_Load("./textures/map/dirt4.png");
    if (dirt_surface == NULL)
        printf("Dirt Texture failed to load\n");
    dirt_texture = SDL_CreateTextureFromSurface(renderer, dirt_surface);
    *(texture_array + 1) = dirt_texture;
    SDL_FreeSurface(dirt_surface);

    /* WALL */
//    SDL_Surface *wall_surface = NULL;
    SDL_Texture *wall_texture = NULL;
    wall_texture = IMG_LoadTexture(renderer, "./textures/map/Dungeon_wall2.jpg");
    if (wall_texture == NULL)
        printf("Wall texture failed to load\n");
//    wall_texture = SDL_CreateTextureFromSurface(renderer, wall_surface);
    *(texture_array + 2) = wall_texture;
//    SDL_FreeSurface(wall_surface);

    /* STAR BACKGROUND */
    SDL_Surface *starbkgd_surface = NULL;
    SDL_Texture *starbkgd_texture = NULL;
    starbkgd_surface = IMG_Load("./textures/map/space1.png");
    if (starbkgd_surface == NULL) 
        printf("starbkgd_texture could not be loaded\n");
    starbkgd_texture = SDL_CreateTextureFromSurface(renderer, starbkgd_surface);
    *(texture_array + 3) = starbkgd_texture;
    SDL_FreeSurface(starbkgd_surface);

    return 1;
}

SDL_Texture * InitializeGreenTexture(SDL_Renderer *renderer) {
    /* Initializes an array of pointers to map textures */
    SDL_Surface *grass_surface = NULL;
    
    // Grass texture
    uint32_t rmask;
    uint32_t bmask;
    uint32_t gmask;
    uint32_t amask;

# if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    grass_surface = SDL_CreateRGBSurface(0, TILE_WIDTH, TILE_HEIGHT, 32,
            rmask, gmask, bmask, amask);

    SDL_Texture *grass_texture = NULL;
    
    grass_texture = SDL_CreateTextureFromSurface(renderer, grass_surface);

    SDL_FreeSurface(grass_surface);

    return grass_texture;
}







