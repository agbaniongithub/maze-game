#include "../headers/raycasting.h"
#include "../headers/world_map.h"
#include <stdio.h>

// Constants for screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Function to handle player input for movement and rotation
void handleInput(const Uint8 *keystate, Player *player, double moveSpeed, double rotSpeed) {
    // Handle keyboard input for movement
    if (keystate[SDL_SCANCODE_W]) { // Move forward
        if (worldMap[(int)(player->posX + player->dirX * moveSpeed)][(int)(player->posY)] == 0)
            player->posX += player->dirX * moveSpeed;
        if (worldMap[(int)(player->posX)][(int)(player->posY + player->dirY * moveSpeed)] == 0)
            player->posY += player->dirY * moveSpeed;
    }
    if (keystate[SDL_SCANCODE_S]) { // Move backward
        if (worldMap[(int)(player->posX - player->dirX * moveSpeed)][(int)(player->posY)] == 0)
            player->posX -= player->dirX * moveSpeed;
        if (worldMap[(int)(player->posX)][(int)(player->posY - player->dirY * moveSpeed)] == 0)
            player->posY -= player->dirY * moveSpeed;
    }
    if (keystate[SDL_SCANCODE_A]) { // Strafe left
        if (worldMap[(int)(player->posX - player->planeX * moveSpeed)][(int)(player->posY)] == 0)
            player->posX -= player->planeX * moveSpeed;
        if (worldMap[(int)(player->posX)][(int)(player->posY - player->planeY * moveSpeed)] == 0)
            player->posY -= player->planeY * moveSpeed;
    }
    if (keystate[SDL_SCANCODE_D]) { // Strafe right
        if (worldMap[(int)(player->posX + player->planeX * moveSpeed)][(int)(player->posY)] == 0)
            player->posX += player->planeX * moveSpeed;
        if (worldMap[(int)(player->posX)][(int)(player->posY + player->planeY * moveSpeed)] == 0)
            player->posY += player->planeY * moveSpeed;
    }
}

// Function to handle mouse movement for rotation

void handleMouse(SDL_Event *e, Player *player, double rotSpeed) {
    if (e->type == SDL_MOUSEMOTION) {
        double deltaX = e->motion.xrel; // Change in mouse x position

        // Horizontal rotation (left and right)
        double angleX = -deltaX * rotSpeed; // Calculate rotation angle around y-axis

        double oldDirX = player->dirX;
        player->dirX = player->dirX * cos(angleX) - player->dirY * sin(angleX);
        player->dirY = oldDirX * sin(angleX) + player->dirY * cos(angleX);

        double oldPlaneX = player->planeX;
        player->planeX = player->planeX * cos(angleX) - player->planeY * sin(angleX);
        player->planeY = oldPlaneX * sin(angleX) + player->planeY * cos(angleX);
    }
}


// The entry point of the application
int main(int argc, char *argv[]) {
    // Initialize SDL video subsystem. Returns 0 on success or a negative error code on failure.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError()); // Print error message if initialization fails
        return 1; // Return non-zero value to indicate an error
    }

    // Create an SDL window titled "3D Maze Game" at position (100, 100) with dimensions 800x600
    SDL_Window *win = SDL_CreateWindow("3D Maze Game", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError()); // Print error message if window creation fails
        SDL_Quit(); // Clean up SDL
        return 1; // Return non-zero value to indicate an error
    }

    // Create an SDL renderer for the window, using accelerated graphics and vertical sync
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win); // Clean up window if renderer creation fails
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError()); // Print error message if renderer creation fails
        SDL_Quit(); // Clean up SDL
        return 1; // Return non-zero value to indicate an error
    }

    // Initialize player position, direction, and camera plane
    Player player = {22, 12, -1, 0, 0, 0.66};

    // Center the mouse and set relative mode for capturing mouse movement
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    double moveSpeed = 0.05; // Movement speed
    double rotSpeed = 0.002; // Rotation speed based on mouse movement

    SDL_Event e; // Declare an SDL_Event structure to handle events
    int quit = 0; // Flag to indicate when the application should quit
    while (!quit) { // Main game loop
        const Uint8 *keystate = SDL_GetKeyboardState(NULL); // Get the current state of the keyboard
        while (SDL_PollEvent(&e)) { // Handle events
            if (e.type == SDL_QUIT) { // If the user closes the window
                quit = 1; // Set quit flag to 1 to exit the loop
            }
            handleMouse(&e, &player, rotSpeed); // Handle mouse movement for rotation
        }
        handleInput(keystate, &player, moveSpeed, rotSpeed); // Handle keyboard input for movement

        // Clear the renderer with a black color
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // Render the 3D view using raycasting
        render3DView(ren, &player);

        SDL_RenderPresent(ren); // Present the current rendering to the screen
    }

    // Clean up resources before quitting
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0; // Return 0 to indicate success
}