#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <SDL2/SDL.h>
#include "world_map.h"

// Define a structure for the player with position, direction, and camera plane vectors
typedef struct {
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} Player;



// Function prototype to render the 3D view using raycasting
void render3DView(SDL_Renderer *renderer, Player *player);

#endif
