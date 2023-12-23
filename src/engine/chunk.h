#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <box2d.h>

#include "smath.h"
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
        void create_cell(i32 x, i32 y, u8 id);
        void apply_draw();

        Chunk* get_neighbour(i32 x, i32 y);
        void set_neighbour(Chunk* neighbour);

        Cell get_cell(i32 x, i32 y);
        void set_cell(i32 x, i32 y, Cell cell);
        bool is_empty(i32 x, i32 y);
        bool in_bounds(i32 x, i32 y) { return x >= 0 && x < size && y >= 0 && y < size; };
        
        vec2i get_world_position() { return {world_x, world_y}; };
        vec2i get_cell_chunk_position(i32 x, i32 y);

        static const u32 size = 120;
        static const u32 area = size * size;
    private:
        //Helper functions
        void set_updated(i32 x, i32 y, u8 value) { update_buffer[y * size + x] = value; };
        bool is_updated(i32 x, i32 y) { return update_buffer[y * size + x] > 0; };
        
        //Cell updates
        void update_bodies(bool has_ticked);
        void update_sand(i32 x, i32 y);
        void update_water(i32 x, i32 y);
        
        //Data
        std::array<Cell, area> cells;
        std::array<u8, area> update_buffer;
        std::array<Cell, area> draw_canvas;

        i32 world_x, world_y;
        std::vector<Chunk*> neighbours;
        std::unordered_map<vec2i, Chunk*, pair_hash> lookup;

        const b2Vec2 gravity = {0.0, 10.0};
        b2World physics_world = b2World(gravity);
        std::vector<Rigidbody> rigidbodies;
};