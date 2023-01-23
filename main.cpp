// # PATH="/Applications/CMake.app/Contents/bin":"$PATH"
// # export CXX=/usr/local/opt/llvm/bin/clang++
// # export CC=/usr/local/opt/llvm/bin/clang
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <stdio.h>
#include <vector>

    
int fps = 60;
unsigned int frame_target_time = 1000/fps;
int ticks_last_frame = 0;


namespace natnat
{

enum class direction {
    left=0,
    right,
    up,
    down
};

class game_object
{
public:
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void switch_texture(float dt) = 0;
    virtual void move(direction d, float dt) = 0;
};

class character: public game_object
{
public:
    character(SDL_Rect src, SDL_Rect dst, SDL_Texture* texture1, SDL_Texture* texture2){
        m_src = src;
        m_dst = dst;
        m_texture1 = texture1;
        m_texture2 = texture2;
    }
    ~character(){
        SDL_DestroyTexture(m_texture1);
        SDL_DestroyTexture(m_texture2);
    }

    void render(SDL_Renderer* renderer){
        if (m_toggle_texture == false){
            SDL_RenderCopy(renderer, m_texture1, &m_src, &m_dst);
        }
        else SDL_RenderCopy(renderer, m_texture2, &m_src, &m_dst);
    }

    void switch_texture(float dt){
        m_time += dt;
        if (m_time >= 0.5){
            m_toggle_texture = !m_toggle_texture;
            m_time = 0;
        }
    }

    void move(direction d, float dt){
        std::cout << dt << " dst: " << m_dst.x << '/' << m_dst.y <<  std::endl;
        switch (d)
        {
        break; case direction::left : m_dst.x-=m_velocity*dt;
        break; case direction::right : m_dst.x+=m_velocity*dt;
        break; case direction::up : m_dst.y-=m_velocity*dt;
        break; case direction::down : m_dst.y+=m_velocity*dt;
        }
    }
private:
    SDL_Rect m_src;
    SDL_Rect m_dst;
    SDL_Texture* m_texture1;
    SDL_Texture* m_texture2;
    int m_velocity = 1000;
    float m_time = 0;
    bool m_toggle_texture = false;
};



class game
{
public:

    game(int width, int height){
        m_window_width = width;
        m_window_height = height;
        m_window = SDL_CreateWindow(
            "Game Window",                  
            SDL_WINDOWPOS_UNDEFINED,           
            SDL_WINDOWPOS_UNDEFINED,           
            m_window_width,                               
            m_window_height,                               
            SDL_WINDOW_OPENGL                  
        );
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        m_running = true;
    }

    ~game(){
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    bool is_running(){
        return m_running;
    }

    void add_character(){
        m_characters.push_back(new character(
            SDL_Rect{0, 0, 300, 230}, 
            SDL_Rect{10, 10, 100, 73}, 
            IMG_LoadTexture(m_renderer, "C:/git/NatNat/bird_down.png"),
            IMG_LoadTexture(m_renderer, "C:/git/NatNat/bird_up.png")));

    }

    void read_input(){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_running = false;
            } 
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        
        if(keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A]) {
            m_characters[0]->move(direction::left, m_dt);
        }
        if(keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D]) {
            m_characters[0]->move(direction::right, m_dt);
        }
        if(keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_W]) {
            m_characters[0]->move(direction::up, m_dt);
        }
        if(keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_S]) {
            m_characters[0]->move(direction::down, m_dt);
        }
    }

    void update(){
        sleep();
        m_characters[0]->switch_texture(m_dt);
    }

    void render(){
        SDL_RenderClear(m_renderer);
        for (auto& bird : m_characters){
            bird->render(m_renderer);
        }
        SDL_RenderPresent(m_renderer);
    }

private:
    void sleep(){
        int time_to_wait = frame_target_time - (SDL_GetTicks()-ticks_last_frame);
        if (time_to_wait > 0 && time_to_wait <= frame_target_time){
            SDL_Delay(time_to_wait);
        }
        m_dt = (SDL_GetTicks() - ticks_last_frame)/1000.0f;
        if (m_dt > 0.05f){
            m_dt = 0.05f;
        }
        ticks_last_frame = SDL_GetTicks();
    }

private:
    SDL_Window* m_window;
    int m_window_width;
    int m_window_height;
    SDL_Renderer* m_renderer;
    bool m_running;
    float m_dt;
    std::vector<game_object*> m_characters;
};

} // namespace natnat



int main(int argc, char* argv[])
{
    natnat::game game(800, 600);

    game.add_character();

    while (game.is_running()){
        game.read_input();
        game.update();
        game.render();
    } 

    return 0;
}
