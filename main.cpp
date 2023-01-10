// # PATH="/Applications/CMake.app/Contents/bin":"$PATH"
// # export CXX=/usr/local/opt/llvm/bin/clang++
// # export CC=/usr/local/opt/llvm/bin/clang
#include "SDL2/SDL.h"
#include <iostream>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
int main(int argc, char* argv[])
{

    SDL_Window *window;                    
    SDL_Init(SDL_INIT_VIDEO);             

    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // A basic main loop to prevent blocking
    bool is_running = true;
    SDL_Event event;
    while (is_running) 
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            } 
            if (event.key.keysym.sym == SDLK_a) {
                std::cout << "a pressed" << std::endl;
            }
            int x;
            int y;
            SDL_GetMouseState(&x, &y);
            std::cout << "mouse pos " << x << '/' << y << std::endl;
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}