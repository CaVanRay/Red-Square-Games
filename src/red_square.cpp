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

    SDL_Rect rect = {100, 100, 200, 200};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Clear to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw red square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // SDL_Rect rect = {100, 100, 200, 200};
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        // getting window size to prevent the square from moving out of bounds
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // Handle keyboard input
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            rect.x -= 1;
            rect.x = std::max(0, rect.x); // prevent moving out of bounds
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            rect.x += 1;
            rect.x = std::min(windowWidth - rect.w, rect.x); // prevent moving out of bounds
        }
        if (keyboardState[SDL_SCANCODE_UP]) {
            rect.y -= 1;
            rect.y = std::max(0, rect.y); // prevent moving out of bounds
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            rect.y += 1;
            rect.y = std::min(windowHeight - rect.h, rect.y); // prevent moving out of bounds
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