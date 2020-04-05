#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

#include "../GameMechanics/gameGrid.h"
#include "spriteSheetOps.h"
#include "../GameMechanics/game_objects.h"

SDL_Surface *horizMirrReflectPixelMap(SDL_Surface *pixelMap) {
    /* Function to perform a horizontal mirror reflection on
     * input pixel map */
    
    /* Initialize a null surface */
    SDL_Surface *flippedPixelMap = NULL;

    /* variable to hold bits per pixel */
    int bitspp;
    int bytespp;
    bitspp = pixelMap->format->BitsPerPixel;
    bytespp = pixelMap->format->BytesPerPixel;

    /* Color masks for pixel surface */
    uint32_t rmask = pixelMap->format->Rmask;
    uint32_t bmask = pixelMap->format->Bmask;
    uint32_t gmask = pixelMap->format->Gmask;

    /* If applicable, initialize a mask for transparency value */
    uint32_t amask;
    if(pixelMap->flags & SDL_TRUE)
        amask = 0;
    else
        amask = pixelMap->format->Amask;


    /* Length of row of pixels in bytes */
    int pitch;
    pitch = pixelMap->pitch;

    /* Get dimensions of pixel map */
    int pix_height;
    int pix_width;
    pix_width = pixelMap->w;
    pix_height = pixelMap->h;

    /* Initialize blank surface to map reversed pixels on */
    flippedPixelMap = SDL_CreateRGBSurface(0, pix_width, pix_height, bitspp, 
            rmask, gmask, bmask, amask);

    /* If the surface must be locked before pixels can be accessed*/     
    if(SDL_MUSTLOCK(pixelMap))
        SDL_LockSurface(pixelMap);

    /* Iterate through input pixel map and copy each pixel 
     * to its corresponding mirrored index */
    uint32_t *flipped_pixels = (uint32_t *) flippedPixelMap->pixels;
    for (int x = 0; x < pix_width; x++) {
        for (int y = 0; y < pix_height; y++) {
            /* Calculate the mirrored coordinates of the pixel to fetch
             * from the original array (y coord will be the same) */
            int mirr_x = (pix_width - 1) - x;
            uint32_t pix_val = getPixel_32bit(mirr_x, y, pixelMap);
            *(flipped_pixels + (y * pix_width) + x) = pix_val;
        }
    }

    // Unlock input surface
    if (SDL_MUSTLOCK(pixelMap))
        SDL_UnlockSurface(pixelMap);

    // Copy color key
    if (pixelMap->flags & SDL_TRUE) {
        uint32_t colorkey = SDL_MapRGB(pixelMap->format, 255, 255, 255);
        SDL_SetColorKey(flippedPixelMap, SDL_RLEACCEL | SDL_TRUE, colorkey);
    }

    return flippedPixelMap;
}

/* Functions to access pixel values */
/* 32-bit */
uint32_t getPixel_32bit(int x, int y, SDL_Surface *pixelMap) {
    // Convert pixels to 32 bit
    uint32_t *pixels = (uint32_t *)pixelMap->pixels;

    //Get requested pixel
    return *(pixels + ((y * pixelMap->w) + x));
}

/* 16-bit */
uint16_t getPixel_16bit(int x, int y, SDL_Surface *pixelMap) {
    // Convert pixels to 32 bit
    uint16_t *pixels = (uint16_t *)pixelMap->pixels;

    // Get requested pixel
    return pixels[(y * pixelMap->w) + x];
}  

/* 8-bit */
uint8_t getPixel_8bit(int x, int y, SDL_Surface *pixelMap) {
    // Convert pixels to 8 bit
    uint8_t * pixels = (uint8_t *)pixelMap->pixels;

    // Get requested pixel
    return pixels[(y * pixelMap->w) + x];
}

