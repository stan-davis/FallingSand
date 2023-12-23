#pragma once

#include "engine/engine.h"
#include "engine/chunk.h"

#include <unordered_map>

class Game : public Engine
{
public:
    Game() = default;
    ~Game() = default;

private:
    void ready() override;
    void update(f32 delta) override;
    void render() override;

    void set_chunk_neighbours();
    
    //Gameplay systems
    u8 world_size = 1;

    std::vector<Chunk*> chunks;
    std::unordered_map<vec2i, Chunk*, pair_hash> lookup;
    vec2i lookup_pos;
    
    //Debug
    DebugDraw debug_draw;
    std::vector<const char*> items = {"Empty", "Sand", "Water", "Stone", "Grass", "Dirt"};
    i32 current_item = 0;
    i32 brush_size = 1;
};