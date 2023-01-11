// # PATH="/Applications/CMake.app/Contents/bin":"$PATH"
// # export CXX=/usr/local/opt/llvm/bin/clang++
// # export CC=/usr/local/opt/llvm/bin/clang
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

enum class direction {
    left=0,
    right,
    up,
    down
};

class character
{
    public:
    character(SDL_Rect src, SDL_Rect dst, SDL_Texture* texture){
        m_src = src;
        m_dst = dst;
        m_texture = texture;
        SDL_QueryTexture(m_texture, NULL, NULL, NULL, NULL);
    }
    ~character(){
        SDL_DestroyTexture(m_texture);
    }

    void render(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, m_texture, &m_src, &m_dst);
    }

    void move(direction d){
        switch (d)
        {
        break; case direction::left : m_dst.x-=20;
        break; case direction::right : m_dst.x+=20;
        break; case direction::up : m_dst.y-=20;
        break; case direction::down : m_dst.y+=20;
        }
    }
    private:
    SDL_Rect m_src;
    SDL_Rect m_dst;
    SDL_Texture* m_texture;
};


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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    character bird(
        SDL_Rect{0, 0, 300, 230}, 
        SDL_Rect{10, 10, 100, 73}, 
        IMG_LoadTexture(renderer, "/Users/nathaliebecker/hellosdl/bird_down.png")
    );

    // A basic main loop to prevent blocking
    bool is_running = true;
    SDL_Event event;

    while (is_running) 
    {
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            } 
            else if (event.key.keysym.sym == SDLK_a) {
                bird.move(direction::left);
            } 
            else if (event.key.keysym.sym == SDLK_s) {
                bird.move(direction::down);
            }
            else if (event.key.keysym.sym == SDLK_d) {
                bird.move(direction::right);
            }
            else if (event.key.keysym.sym == SDLK_w) {
                bird.move(direction::up);
            }
            
            int x;
            int y;
            SDL_GetMouseState(&x, &y);
            //std::cout << "mouse pos " << x << '/' << y << std::endl;
        }
    
        
        bird.render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}