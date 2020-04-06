#ifndef SURVIVORS_H
#define SURVIVORS_H

#include <SDL2/SDL.h>

#include "game_objects.h"

//void *moveSurvivorInWindow (survivor player_char, SDL_Rect *spriteWindowRect);

int checkForTerrainCollision(int pos_x, int pos_y, int **GameGrid);

#endif
