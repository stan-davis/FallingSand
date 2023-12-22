#include "game.h"

void Game::ready()
{
    world = std::make_unique<World>(renderer.get());
}

void Game::update(f32 delta)
{
    if(Input::is_mouse_held(Input::MouseState::BUTTON_LEFT))
    {
        i32 x = std::floor(Input::get_mouse_position().x / 4);
        i32 y = std::floor(Input::get_mouse_position().y / 4);
        
        for(i32 iy = y - brush_size; iy <= y + brush_size; ++iy)
            for(i32 ix = x - brush_size; ix <= x + brush_size; ++ix)
                world->create_cell(ix, iy, current_item);
    }
    else if(Input::is_mouse_released(Input::MouseState::BUTTON_LEFT))
    {
        world->apply_draw();
    }

    //Camera movement
    if(Input::is_key_held(SDL_SCANCODE_D))
    {
        renderer->camera_x += 1;
    }
    else if(Input::is_key_held(SDL_SCANCODE_A))
    {
        renderer->camera_x -= 1;
    }
    if(Input::is_key_held(SDL_SCANCODE_S))
    {
        renderer->camera_y += 1;
    }
    else if(Input::is_key_held(SDL_SCANCODE_W))
    {
        renderer->camera_y -= 1;
    }

    world->update(Time::get_delta_time());
}

void Game::render()
{
    world->render(renderer.get());
    world->render_debug();

    ImGui::Begin("Debug");
    ImGui::Text("FPS: %u", Time::get_frames_per_second());
    ImGui::Text("Delta: %f", Time::get_delta_time());
    ImGui::End();
    
    ImGui::Begin("Interface");
    ImGui::Combo("Materials", &current_item, items.data(), items.size()); ///TODO: fix type saftey of items.data()
    ImGui::SliderInt("Brush size", &brush_size, 1, 8);
    ImGui::End();
    
    renderer->draw_cursor(Input::get_mouse_position().x, Input::get_mouse_position().y, brush_size);
}
