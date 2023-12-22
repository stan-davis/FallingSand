#pragma once

#include <memory>

#include "renderer.h"
#include "time.h"
#include "input.h"

class Engine
{
public:
    Engine() = default;
    ~Engine() = default;
    
    int run(u32 window_width, u32 window_height, u8 resolution);

protected:
    virtual void ready() = 0;
    virtual void update(f32 delta) = 0;
    virtual void render() = 0;

    std::unique_ptr<Renderer> renderer;

private:
    bool running = true;
};