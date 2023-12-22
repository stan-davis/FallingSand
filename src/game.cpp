#include "game.h"
#include <cstdio>

void Game::ready()
{
    debug_draw.SetRenderer(renderer.get());
    debug_draw.SetFlags(b2Draw::e_shapeBit);

    for(i32 x = -world_size; x < world_size; x++)
        for(i32 y = -world_size; y < world_size; y++)
        {
            Chunk* chunk = new Chunk(x * chunk_size, y * chunk_size, &debug_draw);
            vec2i pos = {x, y};
            lookup.insert({pos, chunk});
            chunks.push_back(chunk);
        }
}

void Game::update(f32 delta)
{
    if(Input::is_mouse_held(Input::MouseState::BUTTON_LEFT))
    {
        i32 x = std::floor(Input::get_mouse_position().x / 4) - renderer->camera_x;
        i32 y = std::floor(Input::get_mouse_position().y / 4) - renderer->camera_y;
        
        lookup_pos.x = std::floor((f32)x / chunk_size);
        lookup_pos.y = std::floor((f32)y / chunk_size);

        for(i32 iy = y - brush_size; iy <= y + brush_size; ++iy)
            for(i32 ix = x - brush_size; ix <= x + brush_size; ++ix)
                lookup[lookup_pos]->create_cell(ix - (lookup_pos.x * chunk_size), iy - (lookup_pos.y * chunk_size), current_item);
    }
    else if(Input::is_mouse_released(Input::MouseState::BUTTON_LEFT))
    {
        lookup[lookup_pos]->apply_draw();
    }

    //Camera movement
    if(Input::is_key_held(SDL_SCANCODE_D))
    {
        renderer->camera_x -= 1;
    }
    else if(Input::is_key_held(SDL_SCANCODE_A))
    {
        renderer->camera_x += 1;
    }
    if(Input::is_key_held(SDL_SCANCODE_S))
    {
        renderer->camera_y -= 1;
    }
    else if(Input::is_key_held(SDL_SCANCODE_W))
    {
        renderer->camera_y += 1;
    }

    f32 dt = Time::get_delta_time();

    for(auto& chunk : chunks)
        chunk->update(dt);
}

void Game::render()
{
    for(auto& chunk : chunks)
        chunk->render(renderer.get());

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
