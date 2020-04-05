#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

#include "../GameMechanics/game_objects.h"

void setCameraDisplay(survivor *Player_Character, camera *Camera,
        int **GameGrid, SDL_Texture **textureArray, SDL_Renderer *renderer);

void renderTile(int col, int row, int **GameGrid, 
        SDL_Texture **textureArray, SDL_Rect *renderRect, SDL_Renderer *renderer);

#endif
