#include "../headers/raycasting.h"
#include <math.h>

// Constants for screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Adjust FOV to make walking space narrower
#define FOV_ANGLE (60 * M_PI / 180.0) // Adjust FOV angle as needed

void renderWalls(SDL_Renderer *renderer, Player *player) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate ray direction and starting point
        double cameraX = 2 * x / (double)SCREEN_WIDTH - 1; // Normalize x-coordinate
        double rayDirX = player->dirX + player->planeX * cameraX; // Direction of the ray in x
        double rayDirY = player->dirY + player->planeY * cameraX; // Direction of the ray in y

        // Map coordinates
        int mapX = (int)player->posX;
        int mapY = (int)player->posY;

        // Length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        // Calculate delta distances
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist; // Perpendicular distance to the wall

        // Step direction and initial side distances
        int stepX;
        int stepY;
        int hit = 0; // Hit flag
        int side;    // Was a NS or a EW wall hit?

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player->posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player->posX) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player->posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player->posY) * deltaDistY;
        }

        // Perform DDA
        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Check if ray has hit a wall
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        // Calculate distance to the wall
        if (side == 0) {
            perpWallDist = (mapX - player->posX + (1 - stepX) / 2) / rayDirX;
        } else {
            perpWallDist = (mapY - player->posY + (1 - stepY) / 2) / rayDirY;
        }

        // Calculate height of line to draw on screen
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // Set color based on wall type and side
        Uint8 color = 255;
        if (worldMap[mapX][mapY] == 1) color = 200; // Example: Different color for different wall types
        if (side == 1) color = color / 2; // Darker color for y-side walls

        // Draw wall segment on the screen
        SDL_SetRenderDrawColor(renderer, color, color, color, 255);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}

void renderSkyAndGround(SDL_Renderer *renderer) {
    // Draw ground
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for ground
    SDL_Rect groundRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &groundRect);

    // Draw sky
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky blue color
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &skyRect);
}

void render3DView(SDL_Renderer *renderer, Player *player) {
    renderSkyAndGround(renderer); // Render sky and ground
    renderWalls(renderer, player); // Render walls
}
