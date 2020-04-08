#include <SDL2/SDL.h>
#include <math.h>


#include "../GameMechanics/game_objects.h"
#include "../GameMechanics/stateManagement.h"
#include "camera.h"
#include "../defs.h"

camera  InitializeCamera(int char_posx, int char_posy,
        int height, int width, int x_offset, int y_offset,
        int camera_drift, int char_width, int char_height) {

    static camera Camera;
    Camera.x = char_posx - 600;
    Camera.y = char_posy + 450;
    Camera.height = height;
    Camera.width = width;
    Camera.x_offset = 300;
    Camera.y_offset = 200;
    Camera.camera_drift = 5;

    /* Coordinates of player sprite in view port */
    Camera.spriteCoords.x = char_posx - Camera.x;
    Camera.spriteCoords.y = Camera.y - char_posy;
    Camera.spriteCoords.h = char_height;
    Camera.spriteCoords.w = char_width;

//    camera *Camera_ptr = &Camera;

    return Camera;
}

void setCameraDisplay(survivor **Player_Character, camera **Camera,
        int **GameGrid, SDL_Texture **textureArray, SDL_Renderer *renderer) {
    /* Sets what camera displays based on the position of the player character, 
     * and what will be rendered on screen */

    /* Calculate appropiate camera coordinates */
    calculateCameraCoords(Camera, Player_Character);

    int cam_x = (*Camera)->x;
    int cam_y = (*Camera)->y;

    // Render tiles to map based on the camera's coordinates
    SDL_Rect renderRect; /* Rectangle to keep track of the grid we are rendering */
    renderRect.w = TILE_WIDTH;
    renderRect.h = TILE_HEIGHT;

    /* Retrieve grid coords of top left corner of camera view */
    int grid_x = floor(cam_x / TILE_WIDTH);
    int grid_y = floor(cam_y / TILE_HEIGHT);

    /* height of camera view in tiles */
    int cam_tile_height = floor((*Camera)->height / TILE_HEIGHT);

    /* Starting from top left corner of camera screen, render tiles based on map
     * provided by 'GameGrid' */
    for (int i = cam_tile_height; i > 0; i-=1) {
        renderRect.y = i * TILE_HEIGHT;
        int tile_row = grid_y - (cam_tile_height - i);
        for (int j = 0; j < floor((*Camera)->width / TILE_WIDTH); j+=1) {

            // Update camera rect object to slide render window to next tile
            renderRect.x = j * TILE_WIDTH;

            /* Convert "Camera Grids" into "Game tiles" */
            int tile_col = grid_x + j;
            renderTile(tile_col, tile_row, GameGrid, textureArray, &renderRect, renderer);
            
        }
    }
}

void renderTile(int col, int row, int **GameGrid, 
        SDL_Texture **textureArray, SDL_Rect *renderRect, SDL_Renderer *renderer) {
    /* Renders a single tile on the game window
     * :param col:          column coordinate on game grid of tile to render
     * :param row:          row coordinate on game grid of tile to render
     * :param cameraRect:   coordinates of where tile will be rendered on view window
     * :param textureArray: pointer to an array of pointers which each point to a 
     *                      texture object for the corresponding texture type
     */
    int tile_type; 
//    printf("row: %d\tcol: %d\n", row, col);
    /* If the tile is outside the game map, render a black space tile */
    if ((row >= GAME_HEIGHT) || (row < 0) || (col >= GAME_WIDTH) || (col < 0))
        tile_type = outside_map_tile;

    /* Otherwise, fetch tile type from Game Grid and render that */
    else
        tile_type = *(*(GameGrid) + (row * GAME_HEIGHT) + col);

    // Fetch texture information for tile
    SDL_Texture *texture = *(textureArray + tile_type); 

    // Render texture on game window
    SDL_RenderCopy(renderer, texture, NULL, renderRect);
}


//int renderObject(render_info *objectRenderInfo,  camera *Camera, 
//        SDL_Texture *object_texture, SDL_Renderer *renderer) {
//
//    /* Just render on screen */
//    SDL_Rect test_coords;
//    test_coords.x = 50;
//    test_coords.y = 0;
//    test_coords.w = 50;
//    test_coords.h = 50;
//    SDL_RenderCopy(renderer, object_texture, NULL,
//            &test_coords);
//
//    return 1;
//}

//int renderObject(render_info *objectRenderInfo,  camera *Camera, 
//        SDL_Texture *object_texture, SDL_Renderer *renderer) {
//    /* Determine where to render object on screen */
//    int cam_x = Camera->x;
//    int cam_y = Camera->y;
//
//    int cam_width = Camera->width;
//    int cam_height = Camera->height;
//
//    int object_x = objectRenderInfo->x;
//    int object_y = objectRenderInfo->y;
//
//    int object_width = objectRenderInfo->width;
//    int object_height = objectRenderInfo->height;
//
//    /* Proportion factor for conversion of size of object in game
//     * to size of object in render */
//    int factor_x = objectRenderInfo->factor_x;
//    int factor_y = objectRenderInfo->factor_y;
//
//    SDL_Rect camPosRect; /* Rectangle defining where object appears on screen */
//    SDL_Rect renderRect; /* Rectangle defining what part of object appears on screen */
//
//    /* height and width of object that is actually visible on screen */
//    int y_offset = object_height;
//    int x_offset = object_width;
//
//    /* Determine where the object should appear on screen ,
//     * and what portion of the texture to use */
//    camPosRect.x = object_x - cam_x;
//
//    renderRect.x = 0;
//    renderRect.y = renderRect.h = objectRenderInfo->sheet_height;
//    renderRect.w = objectRenderInfo->sheet_width;
//
//    /* for when the left most porition of the object is offscreen */
//    if (camPosRect.x < 0) {
//        camPosRect.x = 0;
//        x_offset = object_width - (cam_x - object_x);
//        renderRect.x = (object_width - x_offset) * factor_x;
//        renderRect.w = x_offset * factor_x;
//    }
//    else if (object_x + object_width > cam_x + cam_width)  {
//        x_offset = (cam_x + cam_width) - object_x;
//        renderRect.w = x_offset * factor_x;
//    }
//
//    /* For when uppermost portion of object is offscreen */
//    camPosRect.y = object_y - (cam_y - cam_height);
//    if (camPosRect.y > cam_height) {
//        camPosRect.y = cam_height;
//        y_offset = object_height - (object_y - cam_y);
//        renderRect.y = y_offset * factor_y;
//        renderRect.h = renderRect.y;
//    }
//    else if (camPosRect.y - object_height < 0) {
//        y_offset = object_y - (cam_y - cam_height);
//        renderRect.h = y_offset * factor_y;
//    }
//
//    /* Determine proportion of object that is visible to player */
////    float prop_x = (float)(x_offset) / (float)(object_width);
////    float prop_y = (float)(y_offset) / (float)(object_height);
//
////    renderRect.w = x_offset * factor_x;
////    renderRect.h = y_offset * factor_y;
//
////    renderRect.x = (object_width - x_offset) * factor_x;
////    renderRect.y = (object_height - y_offset) * factor_y;
////    renderRect.y = y_offset * factor_y;
//
//    // Render texture on game window
//    SDL_RenderCopy(renderer, object_texture, &renderRect, 
//            &camPosRect);
//
//    return 1;
//}

int renderObject(render_info *objectRenderInfo,  camera *Camera, 
        SDL_Texture *object_texture, SDL_Renderer *renderer) {

   /* Object dimensions in game */
   int object_x = objectRenderInfo->x;
   int object_y = objectRenderInfo->y;
   int object_width = objectRenderInfo->width;
   int object_height = objectRenderInfo->height;

   /* Object texture dimensions */
   int sheet_height = objectRenderInfo->sheet_height;
   int sheet_width = objectRenderInfo->sheet_width;

   /* Camera dimensions */
   int cam_x = Camera->x;
   int cam_y = Camera->y;
   int cam_width = Camera->width;
   int cam_height = Camera->height;

   printf("obj_x: %d\tobj_y: %d\tobj_w: %d\tobj_h: %d\n", 
           object_x, object_y, object_width, object_height);
   printf("cam_x: %d\tcam_y: %d\tcam_w: %d\tcam_h: %d\n", 
           cam_x, cam_y, cam_width, cam_height);

   /* Determine multiplication factor to convert object dimensions
    * in game to dimensions on the texture sheet */
   int factor_x = sheet_width / object_width;
   int factor_y = sheet_height / object_height;

   /* Initialize rectangle to hold coordinates of image on view screen */
   SDL_Rect camCoords;

   /* Initialize rectangle to hold coordinates of portion of texture we
    * will render */
   SDL_Rect rendCoords;

    /* Deal with edge the cases ------*/

    /* If the top of the object is above the view screen */
    if (object_y  < cam_y - cam_height) {
        camCoords.y = 0;

        /* calculate the portion of the object that is visible on screen */
        int y_visible = object_height - ((cam_y - cam_height) - object_y);
        camCoords.h = y_visible;
        rendCoords.y = rendCoords.h = y_visible * factor_y;
    }

    /* If the bottom of the object is below the screen */
    else if (object_y - object_height > cam_y) {
//        printf("bottom of object below view screen\n");
        camCoords.y = cam_height;

        /* Calculate the portion of the object that is visible on screen */
        int y_visible = object_height - (cam_y - object_y);
        camCoords.h = y_visible;
        rendCoords.y = sheet_height;
        rendCoords.h = y_visible * factor_y;
    }
        
    else {
//        printf("full height of object in view screen\n");
        camCoords.y = cam_height - (cam_y - object_y);
        camCoords.h = object_height;
        rendCoords.y = rendCoords.h = sheet_height;
    }

    /* If the left side of the object is off-screen */
    if (object_x < cam_x) {
//        printf("left side of object off view screen\n");
        camCoords.x = 0;

        /* Calculate portion of object visible on screen */
        int x_visible = object_width - (cam_x - object_x);
        camCoords.w = x_visible;
        rendCoords.x = (object_width - x_visible) * factor_x;
        rendCoords.w = sheet_width - rendCoords.x;
    }

    /* If the right side of object is off-screen */
    else if (object_x + object_width > cam_x + cam_width) {
//        printf("right side of object off view screen\n");

        camCoords.x = object_x - cam_x;

        /* Calculate portion of object visible on screen */
        int x_visible = (cam_x + cam_width) - object_x;
        camCoords.w = x_visible;
        rendCoords.x = 0;
        rendCoords.w = x_visible * factor_x;
    }

    else {
//        printf("full width of object on screen\n");
        camCoords.x = object_x - cam_x;
        camCoords.w = object_width;
        rendCoords.x = 0;
        rendCoords.w = sheet_width;
    }
//    printf("rendCoords.x: %d\t, rendCoords.y: %d\t, rendCoords.w: %d\t, rendCoords.h: %d\n", 
//            rendCoords.x, rendCoords.y, rendCoords.w, rendCoords.h);
//    printf("camCoords.x: %d\t, camCoords.y: %d\t, camCoords.w: %d\t, camCoords.h: %d\n", 
//            camCoords.x, camCoords.y, camCoords.w, camCoords.h);
//
//    printf("rendCoords.x: %d\n", rendCoords.x);
//    printf("rendCoords.y: %d\n", rendCoords.y);
//    printf("rendCoords.w: %d\n", rendCoords.w);
//    printf("rendCoords.h: %d\n", rendCoords.h);
//        
//    printf("camCoords.x: %d\n", camCoords.x);
//    printf("camCoords.y: %d\n", camCoords.y);
//    printf("camCoords.w: %d\n", camCoords.w);
//    printf("camCoords.h: %d\n", camCoords.h);

    // Render texture on game window
//    SDL_RenderCopy(renderer, object_texture, &rendCoords, 
//            &camCoords);
    SDL_RenderCopy(renderer, object_texture, NULL, &camCoords);

    return 1;
}

void calculateCameraCoords(camera **Camera, survivor **PlayerCharacter) {

    // Calculate offsets between internally tracked position of character in the world 
    // and the coordinates of the character in the view port as well as the coordinates
    // of the camera itself
    int camera_height = (*Camera)->height;
    int camera_width = (*Camera)->width;
    int cam_x = (*Camera)->x;
    int cam_y = (*Camera)->y;
    int cam_xoffset = (*Camera)->x_offset;
    int cam_yoffset = (*Camera)->y_offset;
    int camera_drift = (*Camera)->camera_drift;

    int char_x = (*Camera)->spriteCoords.x;
    int char_y = (*Camera)->spriteCoords.y;

    int x_velocity = (*PlayerCharacter)->x_velocity;
    int y_velocity = (*PlayerCharacter)->y_velocity;

    if (x_velocity > 0 && ((camera_width - char_x) > cam_xoffset)) 
       /* If the position of the character in the viewport is still outside
        * the bounds of the edge offset, move the character in the viewport
        * but not the camera itself */
        (*Camera)->spriteCoords.x += x_velocity;
    
//    else if (x_velocity < 0 && (char_x <= cam_xoffset))
//        (*Camera)->spriteCoords.x += x_velocity;
    else if (x_velocity < 0 && (char_x > cam_xoffset))
        (*Camera)->spriteCoords.x += x_velocity;

    else if (abs(x_velocity) > 0 && 
            ((camera_width - char_x <= cam_xoffset) ||
             (char_x <= cam_xoffset))) {
        /* If the position of the character in the viewporst is inside
         * the bounds of the edge offset, move the camera coordinates,
         * but not the coordinates of the character in the view port */
        (*Camera)->x += x_velocity;
    }

    else if ((x_velocity == 0) && (char_x != camera_height / 2)) {

        if (abs(char_x - (camera_width / 2)) < camera_drift) {
            /* If the difference between the char's position and the centroid
             * is less than the camera's drift, snap the character to the centroid */
            (*Camera)->x += (*Camera)->spriteCoords.x - camera_width/2;
            (*Camera)->spriteCoords.x = camera_width / 2;
        }

        else if (char_x - (camera_width / 2) < 0) {
            (*Camera)->x -= camera_drift;
            (*Camera)->spriteCoords.x += camera_drift;
        }
        
        else {
            (*Camera)->x += camera_drift;
            (*Camera)->spriteCoords.x -= camera_drift;
        }

    }

    if (y_velocity > 0 && (camera_height - char_y > cam_yoffset)) {
       /* If the position of the character in the viewport is still outside
        * the bounds of the edge offset, move the character in the viewport
        * but not the camera itself */
        (*Camera)->spriteCoords.y += y_velocity;

    }

    else if (y_velocity < 0 && (char_y > cam_yoffset))
        (*Camera)->spriteCoords.y += y_velocity;

    else if (abs(y_velocity) > 0 && ((abs(camera_height - char_y) <= cam_yoffset) ||
                (char_y <= cam_yoffset))) {
        /* If the position of the character in the viewport is inside
         * the bounds of the edge offset, move the camera coordinates,
         * but not the coordinates of the character in the view port */
        (*Camera)->y += y_velocity;

    }

    else if ((y_velocity == 0) && (char_y != camera_height / 2)) {

        if (abs(char_y - (camera_height / 2)) < camera_drift) {
            /* If the difference between the char's position and the centroid
             * is less than the camera's drift, snap the character to the centroid */
            (*Camera)->y += ((*Camera)->height / 2) - (*Camera)->spriteCoords.y;
            (*Camera)->spriteCoords.y = camera_height / 2;
        }

        else if (char_y - (camera_height / 2) < 0) {
            (*Camera)->y -= camera_drift;
            (*Camera)->spriteCoords.y += camera_drift;
        }
        
        else  {
            (*Camera)->y += camera_drift;
            (*Camera)->spriteCoords.y -= camera_drift;
        }

    }

    (*PlayerCharacter)->pos_x = cam_x + char_x;
    (*PlayerCharacter)->pos_y = cam_y - char_y;

//   printf("as assigned cam_x: %d\tcam_y: %d\n",
 //          (*Camera)->x, (*Camera)->y);
}

/* Simple function that returns '1' (true) if object is in view radius,
 * and '0' if object is not */
int isObjectVisible(camera *Camera, int obj_x, int obj_y, int obj_width, int obj_height) {
    /* Check edge cases */
//    printf("obj_x: %d\tobj_y: %d\tobj_width: %d\tobj_height: %d\t\n",
//            obj_x, obj_y, obj_width, obj_height);
//    printf("cam_x: %d\tcam_y: %d\tcam_width: %d\tcam_height: %d\t\n",
//            Camera->x, Camera->y, Camera->width, Camera->height);

    /* Upper left corner */
    if (((obj_x >= Camera->x) && 
         (obj_x < Camera->x + Camera->width)) && 
        ((obj_y - obj_height >= Camera->y - Camera->height) && 
         (obj_y - obj_height < Camera->y)))
        return 1;

    /* Lower left corner */
    else if (((obj_x >= Camera->x) && 
              (obj_x < Camera->x + Camera->width)) &&
            ((obj_y < Camera->y) && 
             (obj_y >= Camera->y - Camera->height)))
        return 1;

    /* Upper right corner */
    else if (((obj_x + obj_width >= Camera->x) && 
              (obj_x + obj_width < Camera->x + Camera->width)) &&
             ((obj_y - obj_height >= Camera->y - Camera->height) &&
              (obj_y - obj_height < Camera->y)))
        return 1;

    /* Lower right corner */
    else if (((obj_x + obj_width >= Camera->x) &&
              (obj_x + obj_width <= Camera->x + Camera->width)) &&
             ((obj_y <= Camera->y) &&
              (obj_y < Camera->y - Camera->height)))
        return 1;

    /* Crazy thought, but what if 1-dim of the object is larger than the
     * screen? Then we need some other way to determine if the object is visible
     */
    if (obj_width > Camera->width) {
        if (((obj_y >= Camera->y - Camera->height) &&
             (obj_y < Camera->y)) ||
            ((obj_y - obj_height >= Camera->y - Camera->height) &&
            (obj_y - obj_height < Camera->y)))
           return 1;
    }

    if (obj_height > Camera->height) {
        if (((obj_x >= Camera->x) &&
             (obj_x < Camera->x + Camera->width)) ||
            ((obj_x + obj_width >= Camera->x) &&
            (obj_x + obj_width < Camera->x + Camera->width)))
            return 1;
    }

    return 0;
}


             






