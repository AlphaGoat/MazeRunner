#ifndef SPRITESHEETOPS_H
#define SPRITESHEETOPS_H

#include "SDL2/SDL.h"

//#include "../GameMechanics/survivors.h"
//
typedef struct Survivor survivor;
typedef struct SS_Coordset ss_coord;

//void getSpriteCoords(survivor *character, int offset, SDL_Rect *spriteRect, 
//        ss_coord *sprite_coords);

SDL_Surface *horizMirrReflectPixelMap(SDL_Surface *pixelMap);

uint32_t getPixel_32bit(int x, int y, SDL_Surface *pixelMap);

uint16_t getPixel_16bit(int x, int y, SDL_Surface *pixelMap);

uint8_t getPixel_8bit(int x, int y, SDL_Surface *pixelMap);

#endif
