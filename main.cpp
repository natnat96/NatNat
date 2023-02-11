// # PATH="/Applications/CMake.app/Contents/bin":"$PATH"
// # export CXX=/usr/local/opt/llvm/bin/clang++
// # export CC=/usr/local/opt/llvm/bin/clang
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map> 

    
int fps = 60;
unsigned int frame_target_time = 1000/fps;
int ticks_last_frame = 0;


namespace natnat
{

/**
 * Components
*/
struct sprite{
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture* texture;
};

struct transform{
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
};
/**
 * Registry
*/
class registry {
public:
    std::size_t create_entity() {
        entity++;
        entities.push_back(entity);
        return entity;
    }
    std::unordered_map<std::size_t, sprite> sprite_components;
    std::unordered_map<std::size_t, transform> transform_components;
    std::vector<std::size_t> entities;

private:
    std::size_t entity{0};
};
 

/**
 * Systems
*/
struct system
{
    virtual void update(registry& r, float dt) {};
    virtual void render(registry& r, SDL_Renderer* renderer) {};
};

struct movement_system : public system
{
    void update(registry& r, float dt) override 
    {
        for (auto& entity : r.entities){
            if (r.transform_components.contains(entity)){
                auto& current = r.transform_components[entity];
                current.pos_x += current.vel_x*dt;
                current.pos_y += current.vel_y*dt;
            }
        }
    }
};

struct sprite_system : public system 
{
    void update(registry& r, float dt) override 
    {
        for (auto& entity : r.entities){
            if (r.transform_components.contains(entity) && r.sprite_components.contains(entity)){
                auto& transform = r.transform_components[entity];
                auto& sprite = r.sprite_components[entity];
                sprite.dst.x = transform.pos_x;
                sprite.dst.y = transform.pos_y;
            }
        }
    }

    void render(registry& r, SDL_Renderer* renderer) override 
    {
        for (auto& entity : r.entities){
            if (r.sprite_components.contains(entity)){
                SDL_RenderCopy(
                    renderer, 
                    r.sprite_components[entity].texture, 
                    &r.sprite_components[entity].src, 
                    &r.sprite_components[entity].dst
                );
            }
        }
    }
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

        m_systems.push_back(new movement_system);
        m_systems.push_back(new sprite_system);

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

    

    void read_input(){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_running = false;
            } 
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
    }

    void update(){
        sleep();
        for (auto& system : m_systems){
            // system = m_systems[i]
            system->update(m_registry, m_dt);
        }
    }

    void render(){
        SDL_RenderClear(m_renderer);

        for (auto& system : m_systems){
            // system = m_systems[i]
            system->render(m_registry, m_renderer);
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

    bool m_running;
    float m_dt;
    std::vector<system*> m_systems;
public:
    registry m_registry;    
    SDL_Renderer* m_renderer;
};


} // namespace natnat


void add_bird(int pos_x , int pos_y , natnat::game& g){
    
}



int main(int argc, char* argv[])
{
    natnat::game game(800, 600);

    std::size_t bird = game.m_registry.create_entity();
    game.m_registry.sprite_components[bird] = natnat::sprite{
            SDL_Rect{0, 0, 300, 230}, 
            SDL_Rect{10, 10, 100, 73}, 
            IMG_LoadTexture(game.m_renderer, "C:/git/NatNat/bird_down.png")
    };
    game.m_registry.transform_components[bird] = natnat::transform{
            100, 100, 10, 10
    };

    std::size_t bird2 = game.m_registry.create_entity();
    game.m_registry.sprite_components[bird2] = natnat::sprite{
            SDL_Rect{0, 0, 300, 230}, 
            SDL_Rect{150, 300, 100, 73}, 
            IMG_LoadTexture(game.m_renderer, "C:/git/NatNat/bird_down.png")
    };

    while (game.is_running()){
        game.read_input();
        game.update();
        game.render();
    } 

    return 0;
}
