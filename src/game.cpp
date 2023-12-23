#include "game.h"
#include <cstdio>

void Game::ready()
{
    debug_draw.SetRenderer(renderer.get());
    debug_draw.SetFlags(b2Draw::e_shapeBit);

    for(i32 x = -world_size; x <= world_size; x++)
        for(i32 y = -world_size; y <= world_size; y++)
        {
            Chunk* chunk = new Chunk(x * Chunk::size, y * Chunk::size, &debug_draw);
            vec2i pos = {x, y};
            lookup.insert({pos, chunk});
            chunks.push_back(chunk);
        }

    set_chunk_neighbours();
}

void Game::update(f32 delta)
{
    if(Input::is_mouse_held(Input::MouseState::BUTTON_LEFT))
    {
        i32 x = std::floor(Input::get_mouse_position().x / 4) + renderer->camera_x;
        i32 y = std::floor(Input::get_mouse_position().y / 4) + renderer->camera_y;
        
        lookup_pos.x = std::floor((f32)x / Chunk::size);
        lookup_pos.y = std::floor((f32)y / Chunk::size);

        for(i32 iy = y - brush_size; iy <= y + brush_size; ++iy)
            for(i32 ix = x - brush_size; ix <= x + brush_size; ++ix)
                lookup[lookup_pos]->create_cell(ix - (lookup_pos.x * Chunk::size), iy - (lookup_pos.y * Chunk::size), current_item);
    }
    else if(Input::is_mouse_released(Input::MouseState::BUTTON_LEFT))
    {
        lookup[lookup_pos]->apply_draw();
    }
    
    //Camera movement
    if(Input::is_key_held(SDL_SCANCODE_D))
    {
        renderer->camera_x += std::round(50.0f * delta);
    }
    else if(Input::is_key_held(SDL_SCANCODE_A))
    {
        renderer->camera_x -= std::round(50.0f * delta);
    }
    if(Input::is_key_held(SDL_SCANCODE_S))
    {
        renderer->camera_y += std::round(50.0f * delta);
    }
    else if(Input::is_key_held(SDL_SCANCODE_W))
    {
        renderer->camera_y -= std::round(50.0f * delta);
    }

    for(auto& chunk : chunks)
        chunk->update(delta);
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

void Game::set_chunk_neighbours()
{
    for(auto& chunk : chunks)
    {
        vec2i world_pos = chunk->get_world_position();
        world_pos.x = std::floor((f32)world_pos.x / Chunk::size);
        world_pos.y = std::floor((f32)world_pos.y / Chunk::size);
        
        for(i32 x = world_pos.x - 1; x <= world_pos.x + 1; x++)
            for(i32 y = world_pos.y - 1; y <= world_pos.y + 1; y++)
            {
                if(x == world_pos.x && y == world_pos.y)
                    continue;
                
                vec2i pos = {x, y};
                auto itr = lookup.find(pos);
                auto end = lookup.end();

                Chunk* neighbour = itr != end ? itr->second : nullptr;
                chunk->set_neighbour(neighbour);
            }
    }
}