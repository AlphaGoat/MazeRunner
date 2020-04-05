

/* Already have game width, height vars in 
 * stateManagement.c .I'll have to come up
 * with a central definition header for all
 * state constants */
#define MAP_WIDTH 40



enum {
    TILE_GRASS,
    TILE_ROAD,
    TILE_BLOCK,
    TILE_GEN,
}

int openTextureFiles()
