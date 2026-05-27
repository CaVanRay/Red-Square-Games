#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <ctime>

int main() {

    // **************************************** WINDOW SETUP **************************************** 

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 720, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> vertDist(-300.0f, 300.0f);

    bool running = true;
    SDL_Event event;


    // getting window size to prevent from moving out of bounds
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_Rect centerLine = { windowWidth / 2, 0, 1, windowHeight};
    SDL_Rect pongBall = { windowWidth / 2, windowHeight / 2, 20, 20};
    SDL_Rect leftPaddle = {20, windowHeight / 2, 20, 200}; 
    SDL_Rect rightPaddle = {windowWidth - 40, windowHeight / 2, 20, 200};

    // VELOCITIES: PIXELS PER SECOND

    float paddleVelocity = 500.0f;
    float ballVertVelocity = 1000.0f;
    float ballHorVelocity = 1000.0f;

    // TIMING VARIABLES

    Uint64 lastTime = SDL_GetTicks64();
    const int TARGET_FPS = 60;
    const float FRAME_DELAY = 1000.0f / TARGET_FPS;


    // **************************************** GAME LOOP **************************************** 

    while (running) {

        Uint64 frameStart = SDL_GetTicks64();
        // 1. CALCULATE DELTA TIME (in seconds)
        Uint64 currentTime = SDL_GetTicks64();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        // Cap deltaTime to prevent huge jumps if lag occurs (e.g. debugging)
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) running = false;
        }

    // **************************************** KEYBOARD INPUT **************************************** 

        // Handle keyboard input
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

        float moveStep = paddleVelocity * deltaTime;
        if (keyboardState[SDL_SCANCODE_UP]) {
             leftPaddle.y -= (int)moveStep;
             rightPaddle.y -= (int)moveStep;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            leftPaddle.y += (int)moveStep;
            rightPaddle.y += (int)moveStep;
        }
        if (keyboardState[SDL_SCANCODE_SPACE]) {
            float boostStep = (paddleVelocity * 3.0f) * deltaTime;
            if(keyboardState[SDL_SCANCODE_UP]){
                leftPaddle.y -= (int)boostStep;
                rightPaddle.y -= (int)boostStep;
            }else if(keyboardState[SDL_SCANCODE_DOWN]){
                leftPaddle.y += (int)boostStep;
                rightPaddle.y += (int)boostStep;
            }            
        }
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }

        // **************************************** MOVEMENT **************************************** 

        // [Clamp Paddles]
        leftPaddle.y = std::max(0, std::min(leftPaddle.y, windowHeight - leftPaddle.h));
        rightPaddle.y = std::max(0, std::min(rightPaddle.y, windowHeight - rightPaddle.h));

        // this will effect ball only
        pongBall.x += (int)(ballHorVelocity * deltaTime);
        pongBall.y += (int)(ballVertVelocity * deltaTime);

        if(SDL_HasIntersection(&pongBall, &leftPaddle)){
            ballHorVelocity = 1000.4f;
            ballVertVelocity = vertDist(gen);
        }
        if(SDL_HasIntersection(&pongBall, &rightPaddle)){
            ballHorVelocity = -1000.0f;
            ballVertVelocity = vertDist(gen);
        }
        if(pongBall.y <= 0 || pongBall.y + pongBall.h >= windowHeight){
            ballVertVelocity = -ballVertVelocity;
        }
        if(pongBall.x < 0 || pongBall.x > windowWidth){
            pongBall.x = windowWidth / 2 - 10;
            pongBall.y = windowHeight / 2;
            ballHorVelocity = (ballHorVelocity > 0) ? 1000.0f : -1000.0f;
            ballVertVelocity = vertDist(gen);
        }

        leftPaddle.y = pongBall.y - 100;
        rightPaddle.y = pongBall.y - 100;


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

        // **************************************** FRAME RATE LIMITING **************************************** 

        Uint64 frameTime = SDL_GetTicks64() - frameStart;
        if(frameTime < FRAME_DELAY){
            SDL_Delay((Uint64)(FRAME_DELAY - frameTime));
        }

    }

    // **************************************** CLEANUP **************************************** 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
