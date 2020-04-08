#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Custom modules
#include "GameMechanics/gameGrid.h"
#include "GameMechanics/game_objects.h"
#include "GameMechanics/survivors.h"
#include "GameMechanics/stateManagement.h"
#include "GraphicsEngine/texture_app.h"

#include "GraphicsEngine/spriteSheetOps.h"
#include "defs.h"

// Modules to be tested
#include "GraphicsEngine/camera.h"


#define FRAMES_PER_SEC 60
#define NUM_GENERATORS 5

time_t frame_init_time;

void printCoords(int pos_x, int pos_y, int camera_x, int camera_y);

int main(int argc, char *argv[]){

    /* seed random number generator */
    srand((unsigned)time(NULL));

    int GameGrid[GAME_WIDTH * GAME_HEIGHT];
    int *GameGrid_ptr = GameGrid;
    /* Initialize Game Grid */
//    allocateGameGrid(GAME_HEIGHT, GAME_WIDTH, &GameGrid);
    InitMapTerrain(&GameGrid_ptr);

    /* Initialize Generators */
//    generator *Generator_Array[NUM_GENERATORS];
//    int gen_flag = 1;
//    int num_generated = 0;
//    while (num_generated < NUM_GENERATORS) {
//        int y = rand() % GAME_HEIGHT;
//        int x = rand() % GAME_WIDTH;
//        for (int i = 0; i < num_generated; i++) {
//            if (x == Generator_Array[i]->pos_x && y == Generator_Array[i]->pos_y)
//                gen_flag = 0;
//                break;
//        }
//        if (gen_flag) {
//            printf("Generator pos: (%d, %d)\n", y, x);
//            generator *gen_ptr;
//            gen_ptr = InitGenerators(x, y); 
//            Generator_Array[num_generated] = gen_ptr;
//            num_generated += 1;
//        }
//    }

    /* Initialize Generator */
    generator Generator;
    Generator = InitGenerators(1500, 1500); 

    render_info genRenderInfo;
    genRenderInfo.x = 1500;
    genRenderInfo.y = 1500;
    genRenderInfo.sheet_width = 582;
    genRenderInfo.sheet_height = 612;
    genRenderInfo.width = 582 / 10;
    genRenderInfo.height = 612 / 10;
    genRenderInfo.factor_x = 10;
    genRenderInfo.factor_y = 10;
    genRenderInfo.texture_flag = enumGeneratorTexture; 

    /* Divide game grid into collision quadrants */
    int num_xcollision_quads = GAME_WIDTH / VIEW_DISTANCE_X;
    int num_ycollision_quads = GAME_HEIGHT / VIEW_DISTANCE_Y;

    int num_collision_quads = num_xcollision_quads * num_ycollision_quads;
    
    // Initialize survivors
    survivor sailorMoon;

    /* Survivor's position on GameGrid */
    sailorMoon.pos_x = 400;
    sailorMoon.pos_y = 400;
    sailorMoon.x_velocity = 0;
    sailorMoon.y_velocity = 0;
    sailorMoon.orientation = SOUTH;
    sailorMoon.health_state = full_health;
    sailorMoon.SpriteRenderInfo.y_offset = 5;
    sailorMoon.SpriteRenderInfo.x_offset = 0;
    sailorMoon.SpriteRenderInfo.sheet_height = 900;
    sailorMoon.SpriteRenderInfo.sheet_width = 386;
    sailorMoon.SpriteRenderInfo.sprite_height = 900 / 20;
    sailorMoon.SpriteRenderInfo.sprite_width = 387 / 18;
    sailorMoon.SpriteRenderInfo.num_motion_frames = 4;
    sailorMoon.SpriteRenderInfo.xmotion_flag = 0;
    sailorMoon.SpriteRenderInfo.ymotion_flag = 0;

//    sailorMoon.collision_quadrant 

    /* Pointer to sailor moon object */
    survivor *sailorMoon_ptr;
    sailorMoon_ptr = &sailorMoon;

//    /* Determine what generators are visible to the player */
//    int visible_generators[NUM_GENERATORS] = {0};
//    for (int i = 0; i < 5; i++) {
//        int view_x = sailorMoon.pos_x;
//        int view_y = sailorMoon.pos_y;
//        int obj_x = (*Generator_Array[i]).pos_x;
//        int obj_y = (*Generator_Array[i]).pos_y;
        int view_radius = 1200;
//

    /* Initialize Camera Object */
    int cam_start_x = sailorMoon.pos_x;
    int cam_start_y = sailorMoon.pos_y;
    int cam_height = 2 * VIEW_DISTANCE_Y;
    int cam_width = 2* VIEW_DISTANCE_X;
    int x_offset = 300;
    int y_offset = 200;
    int camera_drift = 5;
    int char_width = 20;
    int char_height = 30;

    camera Camera = InitializeCamera(cam_start_x, cam_start_y, cam_height,
            cam_width, x_offset, y_offset, camera_drift, char_width, char_height);
    camera *Camera_ptr = &Camera;

//    printf("sheet_width: %d\tsheet_height: %d\n", Generator.renderInfo->sheet_width,
//            Generator.renderInfo->sheet_height);

    int visible_generators_flag = isObjectVisible(&Camera, Generator.pos_x,
            Generator.pos_y, genRenderInfo.width, genRenderInfo.height);

//    camera Camera_Obj;
//    Camera_Obj.x = sailorMoon.pos_x - 600;
//    Camera_Obj.y = sailorMoon.pos_y + 450;
//    Camera_Obj.height = 2 * VIEW_DISTANCE_Y;
//    Camera_Obj.width = 2 * VIEW_DISTANCE_X;
//    Camera_Obj.x_offset = 300;
//    Camera_Obj.y_offset = 200;
//    Camera_Obj.camera_drift = 5;
//    camera *Camera = &Camera_Obj;


  /* Initialize SDL Rectangle defining player character sprite's
   * position in camera view port */
//    Camera_Obj.spriteCoords.h = 30;
//    Camera_Obj.spriteCoords.w = 20;
//    Camera_Obj.spriteCoords.x = 600;
//    Camera_Obj.spriteCoords.y = 450;


    // Initialize game window
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_Window *game_window = SDL_CreateWindow("DBD2EB", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 1200, 900, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(game_window, -1, 0);

    SDL_Event input;

    /* Initialize texture tile array */
    SDL_Texture *texTileArray[4];
    initializeTileTextures(&(texTileArray[0]), renderer);

    /* Initialize Object Texture Array */
    SDL_Texture *texObjectArray[5] = {NULL};
    texObjectArray[enumGeneratorTexture] = initializeGeneratorTexture(renderer);

//    SDL_Rect *infoRect = NULL;
//    SDL_QueryTexture(texObjectArray[enumGeneratorTexture], NULL, NULL,
//            &infoRect->w, &infoRect->h);
//    printf("infoRect_w: %d\tinfoRect_h: %d\n", infoRect->w, infoRect->h);


    /* Initialize sprite sheet */
    SDL_Texture *spriteSheet = NULL;
    SDL_Texture *flipSpriteSheet = NULL;;
    SDL_Surface *temp = IMG_Load("textures/survivors/sailor_moon_spriters-resource_myaphelion.png");

//    printf("sheet_width: %d\tsheet_height: %d\n", Generator.renderInfo->sheet_width,
//            Generator.renderInfo->sheet_height);

    /* Perform horinzontal mirror transform on surface to get additional sprite textures */
    SDL_Surface *temp_flipped = NULL;
    temp_flipped =  horizMirrReflectPixelMap(temp);

    /* Obtain textures from sprite sheet and mirror reflection of sprite sheet
     * to get full moveset */
    spriteSheet = SDL_CreateTextureFromSurface(renderer, temp);
    flipSpriteSheet = SDL_CreateTextureFromSurface(renderer, temp_flipped);

    SDL_FreeSurface(temp);
    SDL_FreeSurface(temp_flipped);


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
    /* debugging flag */
    int print_coords = 0;
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
                        sailorMoon.orientation = WEST;
//                        if (!checkForTerrainCollision(sailorMoon.pos_x - 1,
//                                    sailorMoon.pos_y, &GameGrid_ptr))
                        sailorMoon.x_velocity = -5;
                        sailorMoon.SpriteRenderInfo.xmotion_flag = 1;
//                        print_coords = 1;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        sailorMoon.orientation = EAST;
//                        if (!checkForTerrainCollision(sailorMoon.pos_x + 1,
//                                    sailorMoon.pos_y, &GameGrid_ptr))
                        sailorMoon.x_velocity = 5;
                        sailorMoon.SpriteRenderInfo.xmotion_flag = 1;
 //                       print_coords = 1;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        sailorMoon.orientation = SOUTH;
//                        if (!checkForTerrainCollision(sailorMoon.pos_x,
//                                    sailorMoon.pos_y + 1, &GameGrid_ptr))
                        sailorMoon.y_velocity = 5;
                        sailorMoon.SpriteRenderInfo.ymotion_flag = 1;
//                        print_coords = 1;
                        break;
                    case SDLK_UP:
                    case SDLK_w:
                        sailorMoon.orientation = NORTH;
                       // if (!checkForTerrainCollision(sailorMoon.pos_x,
                       //             sailorMoon.pos_y - 1, &GameGrid_ptr))
                        sailorMoon.y_velocity = -5;
                        sailorMoon.SpriteRenderInfo.ymotion_flag = 1;
//                        print_coords = 1;
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
                        if (sailorMoon.x_velocity < 0) {
                            sailorMoon.x_velocity = 0;
                            sailorMoon.SpriteRenderInfo.xmotion_flag = 0;
                        }
//                        print_coords = 0;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (sailorMoon.x_velocity > 0) {
                            sailorMoon.x_velocity = 0;
                            sailorMoon.SpriteRenderInfo.xmotion_flag = 0;
                        }
//                        print_coords = 0;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        if (sailorMoon.y_velocity > 0) {
                            sailorMoon.y_velocity = 0;
                            sailorMoon.SpriteRenderInfo.ymotion_flag = 0;
                        }
//                        print_coords = 0;
                        break;
                    case SDLK_UP:
                    case SDLK_w:
                        if (sailorMoon.y_velocity < 0) {
                            sailorMoon.y_velocity = 0;
                            sailorMoon.SpriteRenderInfo.ymotion_flag = 0;
                        }
//                        print_coords = 0;
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
        getSpriteCoords(&sailorMoon, 5, &rcSheetSprite);

        /* Update sailor moon's position */
        /* factor in collision detection for edge of map */
//        if (!((sailorMoon.pos_x + sailorMoon.x_velocity >= GAME_WIDTH) ||
//             (sailorMoon.pos_x + sailorMoon.x_velocity < 0)))
//            sailorMoon.pos_x += sailorMoon.x_velocity;
//
//        if (!((sailorMoon.pos_y + sailorMoon.y_velocity >= GAME_HEIGHT) ||
//             (sailorMoon.pos_y + sailorMoon.y_velocity < 0)))
//            sailorMoon.pos_y += sailorMoon.y_velocity;

//        SDL_Rect *spriteWindowCoords;
//        moveSurvivorInWindow(sailorMoon, &spriteWindowCoords);

//        /* Update sailor moon's position in the game window */
//        spriteWindowCoords.x = sailorMoon.pos_x;
//        spriteWindowCoords.y = sailorMoon.pos_y;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        /* Update player character's view camera */
        setCameraDisplay(&sailorMoon_ptr, &Camera_ptr, &GameGrid_ptr, &texTileArray[0],
                renderer);

        /* If a generator is visible, display it */
        if (visible_generators_flag) {
            renderObject(&genRenderInfo, 
                         &Camera, texObjectArray[enumGeneratorTexture], renderer);
        }
        printf("char pos_x, pos_y: %d, %d\n", sailorMoon.pos_x, sailorMoon.pos_y);


//        /* Print coordinates for debugging */
//        printf(" (pos_y, pos_x): (%d, %d)\n", sailorMoon.pos_y, sailorMoon.pos_x);
//        printf(" (cam_y, cam_x): (%d, %d)\n", PlayerCamera.y, PlayerCamera.x);
//        printf(" (cam_pos_y, cam_pos_x): (%d, %d)\n", PlayerCamera.spriteCoords.y,
//                PlayerCamera.spriteCoords.x);
//
//        if (print_coords)
//            printCoords(sailorMoon.pos_x, sailorMoon.pos_y, PlayerCamera.x,
//                    PlayerCamera.y);

        //Copy texture on to window using
        //renderer, sprite rectangle, and window rectangle
        if (sailorMoon.orientation == EAST) 
            /* If survivor is facing right, then we'll need to use flipped sprite sheet */
            SDL_RenderCopy(renderer, flipSpriteSheet, &rcSheetSprite, &Camera_ptr->spriteCoords);
        else
            SDL_RenderCopy(renderer, spriteSheet, &rcSheetSprite, &Camera_ptr->spriteCoords);

//        SDL_Rect test_coords;
//        test_coords.x = 350;
//        test_coords.y = 350;
//        test_coords.w = 50;
//        test_coords.h = 50;
//        SDL_RenderCopy(renderer, texObjectArray[enumGeneratorTexture], NULL,
//                &test_coords);
//        
        SDL_RenderPresent(renderer);

        /* Determine what generators are visible to the player */
        int view_x = sailorMoon.pos_x;
        int view_y = sailorMoon.pos_y;
        int obj_x = Generator.pos_x;
        int obj_y = Generator.pos_y;
//    int obj_width = Generator.renderInfo->width;
//    int obj_height = Generator.renderInfo->height;
        int obj_width = genRenderInfo.width;
        int obj_height = genRenderInfo.height;
        int view_radius = 1200;

//    printf("obj_x: %d\tobj_y: %d\tobj_width: %d\tobj_height: %d\n",
//            obj_x, obj_y, obj_width, obj_height);


        visible_generators_flag = isObjectVisible(&Camera, obj_x, obj_y,
                obj_width, obj_height);
        if (visible_generators_flag)
            printf("generator should be visible\n");
//    printf("visible_generators_flag: %d\n", visible_generators_flag);


//    printf("visible_generators_flag: %d\n", visible_generators_flag);
    


//        printf("Camera (y,x): (%d, %d)", PlayerCamera.y, PlayerCamera.x);


//        SDL_Delay(abs(1000/FRAMES_PER_SEC - (SDL_GetTicks() - frame_init_time)));
        SDL_Delay(abs(2000/FRAMES_PER_SEC - (SDL_GetTicks() - frame_init_time)));
    }

    /* Destroy SDL objects that were created */
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyTexture(flipSpriteSheet);
    for (int i = 0; i < 4; i++)
        if (!(texTileArray[i] == NULL))
            SDL_DestroyTexture(texTileArray[i]);
    for (int i = 0; i < 5; i++)
        if (!(texObjectArray[i] == NULL))
            SDL_DestroyTexture(texObjectArray[i]); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(game_window);
    IMG_Quit();
    SDL_Quit();
//    freeGameGridMem(GameGrid_data, &GameGrid);

    return 1;
}

/* Debugging function */
void printCoords(int pos_x, int pos_y, int camera_x, int camera_y) {
    printf("char (pos_y, pos_x): (%d, %d)", pos_y, pos_x);
    printf("camera (pos_y, pos_x): (%d, %d)", pos_y, pos_x);
}
