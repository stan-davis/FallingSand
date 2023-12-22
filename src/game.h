#pragma once

#include "engine/engine.h"
#include "engine/world.h"

class Game : public Engine
{
public:
    Game() = default;
    ~Game() = default;

private:
    void ready() override;
    void update(f32 delta) override;
    void render() override;

    std::unique_ptr<World> world;

    //Debug GUI
    std::vector<char*> items = {"Empty", "Sand", "Water", "Stone", "Grass", "Dirt"};
    i32 current_item = 0;
    i32 brush_size = 1;
};