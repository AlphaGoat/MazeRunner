#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

enum ErrorType {
    IMAGE_FILE_OPEN_ERROR,
    SDL_INIT_ERROR,
    SDL_RENDER_ERROR,
};

enum ERROR_FATALITY {
    FATAL,
    NONFATAL,
};

const char *ERROR_STRING[] = {
    "IMAGE_FILE_OPEN_ERROR",
    "SDL_INIT_ERROR",
    "SDL_RENDER_ERROR",
};

#endif 
    
