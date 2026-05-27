#include <SDL2/SDL.h>
#include <iostream>

int main() {

    // **************************************** WINDOW SETUP **************************************** 

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Red Square", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // **************************************** GAME VARIABLES **************************************** 

    bool running = true;
    SDL_Event event;

    SDL_Rect redSquare = {960, 400, 40, 40}; // Initial position and size of the red square
    SDL_Rect ghostSquare = {960, 400, 40, 40}; // Ghost square for collision detection
    SDL_Rect bluePlatform = {960, 1200, 400, 40}; // Blue platform in center of the screen
    SDL_Rect greenPlatform = {500, 1000, 400, 40}; // Green platform on the left side of the screen
    SDL_Rect yellowPlatform = {1100, 850, 400, 40}; // Yellow platform on the right side of the screen

    float verticalVelocity = 0.0f; // Speed of the red square
    const float GRAVITY = 0.2f; // Gravity affecting the red square
    bool onGround = true; // To check if the red square is on the ground for jumping

    // **************************************** GAME LOOP **************************************** 

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // getting window size to prevent the square from moving out of bounds
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        

        float horizontalVelocity = 0.0f; // Horizontal velocity reset each frame
        verticalVelocity += GRAVITY; // Apply gravity to vertical velocity

    // **************************************** KEYBOARD INPUT **************************************** 

        // Handle keyboard input
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            horizontalVelocity = -5.0f; 
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            horizontalVelocity = 5.0f;
        }
        if (keyboardState[SDL_SCANCODE_SPACE] && onGround) {
            verticalVelocity -= 12.0f; 
            onGround = false;
        }
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }

        // **************************************** HORIZONTAL MOVEMENT **************************************** 
        ghostSquare.x += horizontalVelocity;
        ghostSquare.x = std::max(0, std::min(windowWidth - ghostSquare.w, ghostSquare.x)); // prevent moving out of bounds
        if (SDL_HasIntersection(&ghostSquare, &bluePlatform)|SDL_HasIntersection(&ghostSquare, &greenPlatform)|SDL_HasIntersection(&ghostSquare, &yellowPlatform)) {
            ghostSquare.x -= horizontalVelocity; // undo movement if colliding with blue platform
        }

        // **************************************** VERTICAL MOVEMENT **************************************** 
        ghostSquare.y += verticalVelocity;
        ghostSquare.y = std::min(windowHeight - ghostSquare.h, ghostSquare.y);

        bool hitWall = false; // Flag to check if we hit the wall
        if  (SDL_HasIntersection(&ghostSquare, &bluePlatform)|SDL_HasIntersection(&ghostSquare, &greenPlatform)|SDL_HasIntersection(&ghostSquare, &yellowPlatform)) {
            ghostSquare.y -= verticalVelocity; // undo movement if colliding with blue platform
            if (verticalVelocity > 0) { // If falling down, we are on the ground
                onGround = true;
                verticalVelocity = 0; // Stop vertical movement when hitting the ground
                hitWall = true;
            } else if (verticalVelocity < 0) { // If moving up, we hit the ceiling
                verticalVelocity = 0; // Stop vertical movement when hitting the ceiling
            }   hitWall = true;
        }

        if (!hitWall && ghostSquare.y >= windowHeight - ghostSquare.h) {
            onGround = true; // We are on the ground if we hit the bottom of the window
            verticalVelocity = 0; // Stop vertical movement when hitting the ground
        }

        // **************************************** UPDATE RED SQUARE LOCATION **************************************** 
        redSquare.x = ghostSquare.x;
        redSquare.y = ghostSquare.y;

        // **************************************** RENDERING **************************************** 

        // Clear to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // Draw red square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // SDL_Rect redSquare = {100, 100, 200, 200};
        SDL_RenderFillRect(renderer, &redSquare);
        // Draw blue platform
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &bluePlatform);
        // Draw green platform
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &greenPlatform);
        // Draw yellow platform
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &yellowPlatform);

        SDL_RenderPresent(renderer);

    }

    // **************************************** CLEANUP **************************************** 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
