#include "engine.h"
#include <cstdio>

int Engine::run(u32 window_width, u32 window_height, u8 resolution)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return -1;
    }

    renderer = std::make_unique<Renderer>(window_width, window_height, resolution);

    ready();

    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            
            switch(event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
            }
        }

        Input::update();
        Time::update();
        
        f32 delta = Time::get_delta_time();
        update(delta);
        
        renderer->start_frame();
        renderer->render();
        render();
        renderer->end_frame();

        Input::late_update();
        Time::late_update();
    }

    SDL_Quit();

    return 0;
}