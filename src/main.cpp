#include <stdio.h>
#include <memory>
#include <math.h>
#include <SDL.h>

#include "engine/renderer.h"
#include "engine/world.h"
#include "engine/time.h"
#include "engine/input.h"

int main(int argc, char** argv)
{
    //Init
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(640, 480, 4);
    std::unique_ptr<World> world = std::make_unique<World>(renderer.get());
    std::unique_ptr<Time> time = std::make_unique<Time>(75);
    std::unique_ptr<Input> input = std::make_unique<Input>();
    
    //Loop
    //SDL_ShowCursor(SDL_DISABLE);
    bool running = true;

    const char* items[] = {"Empty", "Sand", "Water", "Stone"};
    int current_item = 0;
    int brush_size = 1;
    
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

        input->update();
        time->update();
        
        if(input->is_mouse_held(MouseState::BUTTON_LEFT))
        {
            i32 x = std::floor(input->get_mouse_x() / 4);
            i32 y = std::floor(input->get_mouse_y() / 4);
            
            for(int iy = y - brush_size; iy <= y + brush_size; ++iy)
                for(int ix = x - brush_size; ix <= x + brush_size; ++ix)
                    world->create_cell(ix, iy, current_item);
        }
        else if(input->is_mouse_released(MouseState::BUTTON_LEFT))
        {
            world->apply_draw();
        }

        world->update(time->get_delta_time());
        world->render(renderer.get());

        renderer->start_frame();

        ImGui::Begin("Debug");
        ImGui::Text("FPS: %u", time->get_frames_per_second());
        ImGui::Text("Delta: %f", time->get_delta_time());
        ImGui::End();
        
        ImGui::Begin("Interface");
        ImGui::Combo("Materials", &current_item, items, IM_ARRAYSIZE(items));
        ImGui::SliderInt("Brush size", &brush_size, 1, 8);
        ImGui::End();
        
        renderer->render();
        renderer->draw_cursor(input->get_mouse_x(), input->get_mouse_y(), brush_size);
        world->render_debug();
        renderer->end_frame();
        
        input->late_update();
        time->late_update();
    }

    SDL_Quit();

    return 0;
}
