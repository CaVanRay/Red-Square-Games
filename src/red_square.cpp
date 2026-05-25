#include <SDL2/SDL.h>
#include <iostream>

int main() {
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

    bool running = true;
    SDL_Event event;

    SDL_Rect redSquare = {10, 10, 20, 20}; // Initial position and size of the red square
    SDL_Rect ghostSquare = {10, 10, 20, 20}; // Ghost square for collision detection
    SDL_Rect blueWall = {960, 0, 20, 540}; // Blue wall in center of the screen

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Clear to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw red square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // SDL_Rect redSquare = {100, 100, 200, 200};
        SDL_RenderFillRect(renderer, &redSquare);
        // Draw blue wall
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &blueWall);

        SDL_RenderPresent(renderer);

        // getting window size to prevent the square from moving out of bounds
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // Handle keyboard input
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            ghostSquare.x -= 1;
            ghostSquare.x = std::max(0, ghostSquare.x); // prevent moving out of bounds
            if (SDL_HasIntersection(&ghostSquare, &blueWall)) {
                ghostSquare.x += 1; // undo movement if colliding with blue wall
            } else {
                redSquare.x = ghostSquare.x; // update red square position if not colliding
            }
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            ghostSquare.x += 1;
            ghostSquare.x = std::min(windowWidth - ghostSquare.w, ghostSquare.x); // prevent moving out of bounds
            if (SDL_HasIntersection(&ghostSquare, &blueWall)) {
                ghostSquare.x -= 1; // undo movement if colliding with blue wall
            } else {
                redSquare.x = ghostSquare.x; // update red square position if not colliding
            }
        }
        if (keyboardState[SDL_SCANCODE_UP]) {
            ghostSquare.y -= 1;
            ghostSquare.y = std::max(0, ghostSquare.y); // prevent moving out of bounds
                if (SDL_HasIntersection(&ghostSquare, &blueWall)) {
                    ghostSquare.y += 1; // undo movement if colliding with blue wall
                } else {
                    redSquare.y = ghostSquare.y; // update red square position if not colliding
                }
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            ghostSquare.y += 1;
            ghostSquare.y = std::min(windowHeight - ghostSquare.h, ghostSquare.y); // prevent moving out of bounds
            if (SDL_HasIntersection(&ghostSquare, &blueWall)) {
                ghostSquare.y -= 1; // undo movement if colliding with blue wall
            } else {
                redSquare.y = ghostSquare.y; // update red square position if not colliding
            }
        }
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }



    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}