#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

#include "../GameMechanics/game_objects.h"

/* Initialize a structure for the player's camers */
typedef struct Camera {
    /* position of top left view tile in Game Grid */
    int x;
    int y;

    /* width and height of camera window, in pixels */
    int width;
    int height;

    /* offset of character to edges of camera screen */
    int x_offset;
    int y_offset;

    /* How quickly the camera drifts back to be centered on the
     * player character's position when the player is idle */
    int camera_drift;

    /* Rect object defining the placement of the character's 
     * object with respect to the camera view port*/
    SDL_Rect spriteCoords;

} camera;

camera  InitializeCamera(int start_posx, int start_posy,
        int height, int width, int x_offset, int y_offset,
        int camera_drift, int char_width, int char_height);

void setCameraDisplay(survivor **Player_Character, camera **Camera,
        int **GameGrid, SDL_Texture **textureArray, SDL_Renderer *renderer);

void renderTile(int col, int row, int **GameGrid, 
        SDL_Texture **textureArray, SDL_Rect *renderRect, SDL_Renderer *renderer);

void calculateCameraCoords(camera **Camera, survivor **Player_Character);

int renderObject(render_info *objectRenderInfo, camera *Camera, SDL_Texture *object_texture,
        SDL_Renderer *renderer);

int isObjectVisible(camera *Camera, int obj_posx, int obj_posy, int obj_width,
        int obj_height);

#endif
