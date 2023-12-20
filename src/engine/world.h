#pragma once

#include <array>
#include <vector>
#include <box2d.h>

#include "cell.h"
#include "rigidbody.h"
#include "renderer.h"
#include "utils/debug_draw.h"

class World
{
    public:
        World(Renderer* _renderer);
        ~World() = default;

        void update(f32 dt);
        void render(Renderer *renderer);
        void render_debug();
        void create_cell(u16 x, u16 y, u8 id);

        void apply_draw();
    private:
        //Helper functions
        Cell get_cell(u16 x, u16 y);
        void set_cell(u16 x, u16 y, Cell cell);
        bool is_empty(u16 x, u16 y);
        bool in_bounds(u16 x, u16 y);
        void set_updated(u16 x, u16 y, u8 value);
        bool is_updated(u16 x, u16 y);

        //Cell updates
        void update_bodies(bool has_ticked);
        void update_sand(u16 x, u16 y);
        void update_water(u16 x, u16 y);
        
        //Data
        static const u16 size = 120;
        static const u16 area = size * size;
        
        std::array<Cell, area> cells;
        std::array<u8, area> update_buffer;
        std::array<Cell, area> draw_canvas;

        const b2Vec2 gravity = {0.0, 10.0};
        b2World physics_world = b2World(gravity);
        DebugDraw debug_draw;
        std::vector<Rigidbody> rigidbodies;
};