#include <SDL2/SDL.h>
#include <iostream>

int main() {

    // **************************************** WINDOW SETUP **************************************** 

    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
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

    SDL_Rect centerLine = { 640, 640, 720, 10};
    SDL_Rect pongBall = { 640, 640, 20, 20};
    SDL_Rect leftPaddle = {360, 80, 20, 200}; 
    SDL_Rect rightPaddle = {360, 1200, 20, 200};


    // **************************************** GAME LOOP **************************************** 

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // getting window size to prevent from moving out of bounds
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // **************************************** KEYBOARD INPUT **************************************** 

        // Handle keyboard input
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_UP]) {
             
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            horizontalVelocity = 5.0f;
        }
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }

        // **************************************** HORIZONTAL MOVEMENT **************************************** 

        // this will effect ball only

        // **************************************** VERTICAL MOVEMENT **************************************** 

        // this will effect ball & paddles

        // **************************************** RENDERING **************************************** 

        // Clear to black and make background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // Change color to white and render center line, paddles, & ball
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &centerLine);
        SDL_RenderFillRect(renderer, &pongBall);
        SDL_RenderFillRect(renderer, &leftPaddle);
        SDL_RenderFillRect(renderer, &rightPaddle);
       

        SDL_RenderPresent(renderer);

    }

    // **************************************** CLEANUP **************************************** 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
