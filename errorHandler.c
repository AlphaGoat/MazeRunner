#include <stdio.h>
#include "errorHandler.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

char *provideError(char *funcName, int errorType) {
    printf("Error type %s encountered in %s\n", funcName, ERROR_STRING[errorType]);
    if (isErrorFatal(errorType)) {
        printf("FATAL ERROR ENCOUNTERED. EXITING.");
    }
}


    

int isErrorFatal(int errorType) {
    switch(errorType){
        case IMAGE_FILE_OPEN_ERROR:
        case SDL_RENDER_ERROR:
            return NONFATAL;
        case SDL_INIT_ERROR:
            return FATAL;
    }
}

int destroy_sdl_objects(int num_open_surfaces, int num_open_textures, 
        SDL_Surface *openSurfaces, SDL_Texture *openTextures,
        SDL_Renderer *renderer, SDL_Window *window) {
    /* Function destroys all SDL objects and exits SDL */
    printf("Freeing SDL_Surfaces...\n");
    for (int i = 0; i < num_open_surfaces; i++) {
        SDL_Surface *surfaceToDestroy;
        surfaceToDestroy = &openSurfaces[i];
        SDL_FreeSurface(surfaceToDestroy);
    }

    printf("SDL_Surfaces freed\n");
    printf("Destroying SDL_Textures...\n");
    for (int j = 0; j < num_open_textures; j++) {
        SDL_Texture *textureToDestroy;
        textureToDestroy = &openTextures[j];
        SDL_DestroyTexture(textureToDestroy);
    }

    printf("SDL_Textures destroyed\n");
    printf("Destroying SDL_Renderer...\n");
    SDL_DestroyRenderer(renderer);

    printf("SDL_Renderer destroyed\n");
    printf("Destroying SDL_Window...\n");
    SDL_DestroyWindow(window);

    printf("SDL_Window destroyed\n");
    printf("Exiting SDL_Image\n");
    IMG_Quit();

    printf("Exiting SDL\n");
    SDL_Quit();

    printf("All SDL objects succesfully destroyed.\n");
    return 1;
}





