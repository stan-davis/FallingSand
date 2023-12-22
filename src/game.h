#pragma once

#include "engine/engine.h"
#include "engine/chunk.h"

class Game : public Engine
{
public:
    Game() = default;
    ~Game() = default;

private:
    void ready() override;
    void update(f32 delta) override;
    void render() override;

    //Gameplay systems
    std::vector<Chunk*> chunks;
    
    //Debug
    DebugDraw debug_draw;
    std::vector<char*> items = {"Empty", "Sand", "Water", "Stone", "Grass", "Dirt"};
    i32 current_item = 0;
    i32 brush_size = 1;
};