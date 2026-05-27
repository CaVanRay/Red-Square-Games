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

    SDL_Rect centerLine = { 1280, 0, 1, 2800};
    SDL_Rect pongBall = { 1270, 740, 20, 20};
    SDL_Rect leftPaddle = {20, 640, 20, 200}; 
    SDL_Rect rightPaddle = {2520, 640, 20, 200};

    float LeftVertVelocity = 0.0f;
    float rightVertVelocity = 0.0f;
    float ballVertVelocity = 0.0f;
    float ballHorVelocity = 25.0f;


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
             leftPaddle.y -= 10;
             rightPaddle.y -= 10;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            leftPaddle.y += 10;
            rightPaddle.y += 10;
        }
        if (keyboardState[SDL_SCANCODE_SPACE]) {
            if(keyboardState[SDL_SCANCODE_UP]){
                leftPaddle.y -= 30;
                rightPaddle.y -= 30;
            }else if(keyboardState[SDL_SCANCODE_DOWN]){
                leftPaddle.y += 30;
                rightPaddle.y += 30;
            }            
        }
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }

        // **************************************** HORIZONTAL MOVEMENT **************************************** 

        // this will effect ball only
        pongBall.x += ballHorVelocity;

        if(SDL_HasIntersection(&pongBall, &leftPaddle)){
            ballHorVelocity = 25;
        }
        if(SDL_HasIntersection(&pongBall, &rightPaddle)){
            ballHorVelocity = -25;
        }
        if(pongBall.x < 0 || pongBall.x > windowWidth){
            pongBall.x = 1270;
        }

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
