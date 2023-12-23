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
        void create_cell(u16 x, u16 y, u8 id);
        void apply_draw();

        Chunk* get_neighbour(i32 x, i32 y);
        void set_neighbour(Chunk* neighbour);

        Cell get_cell(u16 x, u16 y);
        void set_cell(u16 x, u16 y, Cell cell);
        bool is_empty(u16 x, u16 y);
        bool in_bounds(u16 x, u16 y) { return x >= 0 && x < size && y >= 0 && y < size; };
        
        vec2i get_world_position() { return {world_x, world_y}; };
        vec2i get_cell_chunk_position(i32 x, i32 y);

        static const u16 size = 120;
        static const u16 area = size * size;
    private:
        //Helper functions
        void set_updated(u16 x, u16 y, u8 value) { update_buffer[y * size + x] = value; };
        bool is_updated(u16 x, u16 y) { return update_buffer[y * size + x] > 0; };
        
        //Cell updates
        void update_bodies(bool has_ticked);
        void update_sand(u16 x, u16 y);
        void update_water(u16 x, u16 y);
        
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