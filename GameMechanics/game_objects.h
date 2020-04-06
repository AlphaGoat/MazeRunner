#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <time.h>

#include "../GraphicsEngine/texture_app.h"

typedef struct Survivor {
    /* Position of top left tile of char in GameGrid */
    int pos_x;
    int pos_y;

    /* Dims of character, in tiles */
    int width;
    int height;

    /* Velocity vector information */
    int x_velocity;
    int y_velocity;
    int total_velocity;

    /* physical orientation (which direction player
     * is facing on game map). Tells renderer which
     * sprite set to use */
    int orientation;

    int health_state; /* health status of survivor */
    int alive_flag;   /* 1 if survivor is alive, 0 if dead */ 

    int hooked_flag;  /* Flag to see if survivor is hanging on hook */
    int hook_status;  /* Another flag to see what 'stage' of hooked survivor is in */

    time_t idle_timer; /* timer for how long the character has been idle */


    /* Pointer to appropiate sprite sheet structure
     * containing rendering information for character */
    sprite_render_info *SpriteRenderInfo;

} survivor;

typedef struct Killer {
    /* Position on Game Grid */
    int pos_x;
    int pos_y;
    /* Velocity vector information */
    int x_velocity;
    int y_velocity;
} killer;

typedef struct Generator {
    /* Position of top left tile of generator on GameGrid */
    int pos_x;
    int pos_y;

    /* dims of generator, in tiles */
    int width;
    int height;

    /* Fixed Flag (1 if fixed, 0 if not) */
    int fixed;

    /* bar determining how close to fixed 
     * status the generator is. When this
     * hits 0, the generator is 'fixed' */
    int completion_bar;
} generator;

typedef struct Gate {
    /* position on game grid */
    int pos_x;
    int pos_y;

    int open_flag; /* 1 if gate is open, 0 if closed */

    // Bar detailing how close to open the gate is
    // (100 being fully closed, 0 being fully open)
    int completion_bar;
} gate;

typedef struct Hatch {
    /* position on game grid */
    int pos_x;
    int pos_y;

    int open_flag; /* 1 if gate is open, 0 if closed */
} hatch;


enum HealthStates {
    full_health,
    lightly_wounded,
    heavily_wounded,
} health_states;

#endif
