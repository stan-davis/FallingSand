#pragma once

#include <array>
#include <vector>
#include <box2d.h>

#include "cell.h"
#include "rigidbody.h"
#include "renderer.h"
#include "debug_draw.h"

class Chunk
{
    public:
        Chunk(i32 x, i32 y, DebugDraw* debug_draw);
        ~Chunk() = default;

        void update(f32 dt);
        void render(Renderer *renderer);
        void create_cell(u16 x, u16 y, u8 id);
        void apply_draw();

    private:
        //Helper functions
        Cell get_cell(u16 x, u16 y) { return cells[y * size + x]; };
        void set_cell(u16 x, u16 y, Cell cell) { if(in_bounds(x,y)) cells[y * size + x] = cell; };
        bool is_empty(u16 x, u16 y) { return cells[y * size + x].id == 0; };
        bool in_bounds(u16 x, u16 y) { return x >= 0 && x < size && y >= 0 && y < size; };
        void set_updated(u16 x, u16 y, u8 value) { update_buffer[y * size + x] = value; };
        bool is_updated(u16 x, u16 y) { return update_buffer[y * size + x] > 0; };

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

        i32 world_x;
        i32 world_y;

        const b2Vec2 gravity = {0.0, 10.0};
        b2World physics_world = b2World(gravity);
        std::vector<Rigidbody> rigidbodies;
};