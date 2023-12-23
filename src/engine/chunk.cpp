#include "chunk.h"
#include "smath.h"

#include <cstdio>
#include <FastNoiseLite.h>
#include <algorithm>

Chunk::Chunk(i32 x, i32 y, DebugDraw* debug_draw)
{
    world_x = x;
    world_y = y;
    
    physics_world.SetDebugDraw(debug_draw);
    
    //Terrain generation
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    u32 amplitude = 20;

    for(i32 y = 0; y < size; y++)
        for(i32 x = 0; x < size; x++)
        {
            i32 tx = world_x + x;
            i32 ty = world_y + y;

            u16 r = rand() % 4 + 1;
            i32 depth = std::ceil(noise.GetNoise((f32)tx, 0.0f) * amplitude);

            if(ty > depth)
                create_cell(x, y, CellType::GRASS);
            if(ty > depth + r)
                create_cell(x, y, CellType::DIRT);
        }

    apply_draw();
}

Chunk* Chunk::get_neighbour(i32 x, i32 y)
{
    vec2i pos = {std::floor((f32)x / size), std::floor((f32)y / size)};

    auto itr = lookup.find(pos);
    auto end = lookup.end();
    
    return itr != end ? itr->second : nullptr;
}

void Chunk::set_neighbour(Chunk* neighbour)
{
	if (neighbour == nullptr)
		return;

	vec2i pos = {std::floor((f32)neighbour->get_world_position().x / size), std::floor((f32)neighbour->get_world_position().y / size)};
	lookup.insert({ pos, neighbour });
	neighbours.push_back(neighbour);
}

vec2i Chunk::get_cell_chunk_position(i32 x, i32 y)
{
    return { x - world_x , y - world_y };
}

///TODO: this will never go negative because it is an unsigned int (replace all functions with i32)
Cell Chunk::get_cell(i32 x, i32 y)
{
    if(in_bounds(x, y))
    {
        return cells[y * size + x]; 
    }

    i32 world_pos_x = x + world_x;
    i32 world_pos_y = y + world_y;
        
    if(Chunk* dst = get_neighbour(world_pos_x, world_pos_y))
    {
        vec2i to = dst->get_cell_chunk_position(world_pos_x, world_pos_y);
        return dst->get_cell(to.x, to.y);
    }
    
    return Cell();
}

void Chunk::set_cell(i32 x, i32 y, Cell cell)
{
    if(in_bounds(x, y))
    {
        cells[y * size + x] = cell;
    }
    else
    {
        i32 world_pos_x = x + world_x;
        i32 world_pos_y = y + world_y;
        
        if(Chunk* dst = get_neighbour(world_pos_x, world_pos_y))
        {
            vec2i to = dst->get_cell_chunk_position(world_pos_x, world_pos_y);
            dst->set_cell(to.x, to.y, cell); ///+1 for testing ONLY
        }
    }
}



bool Chunk::is_empty(i32 x, i32 y)
{
    if(in_bounds(x, y))
    {
        return cells[y * size + x].id == 0;
    }

    i32 world_pos_x = x + world_x;
    i32 world_pos_y = y + world_y;
        
    if(Chunk* dst = get_neighbour(world_pos_x, world_pos_y))
    {
        vec2i to = dst->get_cell_chunk_position(world_pos_x, world_pos_y);
        return dst->get_cell(to.x, to.y).id == 0;
    }
    
    return false;
}

void Chunk::create_cell(i32 x, i32 y, u8 id)
{
    Color color;
    u8 r = rand() % 64;

    switch(id)
    {
        case CellType::EMPTY: break;
        case CellType::SAND: color = {u8(255 - r), u8(255 - r), 0}; break;
        case CellType::WATER: color = {0, 64, 255}; break;
        case CellType::STONE: color = {u8(128 - r), u8(128 - r), u8(128 - r)}; break;
        case CellType::GRASS: color = {0, u8(160 - r), u8(64 - r)}; break;
        case CellType::DIRT: color = {u8(104 - r), u8(64 - r), u8(64 - r)}; break;
    }

    Cell cell = {id, color, 0, 0};

    if(!in_bounds(x, y))
        return;
    
    draw_canvas[y * size + x] = cell;
}

void Chunk::apply_draw()
{
    if(draw_canvas.empty())
        return;
    
    b2BodyType type = b2_staticBody;
    bool found = false;

    for(auto& cell : draw_canvas)
    {
        u8 id = cell.id;
        
        if(id == CellType::STONE || id == CellType::GRASS || id == CellType::DIRT)
        {
            if(id == CellType::STONE)
                type = b2_dynamicBody;

            found = true;
        }
    }

    if(found)
    {
        std::vector<Cell> data(draw_canvas.begin(), draw_canvas.end());
        Rigidbody rb = Rigidbody(data, type, {(f32)world_x, (f32)world_y}, physics_world);
        rigidbodies.push_back(rb);
    }

    for(i32 i = 0; i < area; i++)
    {
        if(draw_canvas[i].id != CellType::EMPTY)
            cells[i] = draw_canvas[i];
    }
    
    draw_canvas.fill(Cell());
}

void Chunk::render(Renderer *renderer)
{
    for(u32 i = area; i > 0; --i)
    {
        u32 x = std::floor(i % size);
        u32 y = std::floor(i / size);

        Cell buffer_cell = get_cell(x, y);
        Cell canvas_cell = draw_canvas[i];
        u32 c = buffer_cell.color.to_uint();

        if(canvas_cell.id != CellType::EMPTY)
            c = canvas_cell.color.to_uint();

        renderer->draw(world_x + x, world_y + y, c);
    }
    
    physics_world.DebugDraw();
}

///TODO: still some issues with in-place updates
void Chunk::update(f32 dt)
{
    for(u32 i = area; i > 0; --i)
    {
        u32 x = std::floor(i % size);
        u32 y = std::floor(i / size);

        if(is_updated(x,y))
            continue;
        
        const Cell& cell = get_cell(x, y);

        switch(cell.id)
        {
            case CellType::SAND: update_sand(x, y); break;
            case CellType::WATER: update_water(x, y); break;
            default: break;
        }
    }

    memset(&update_buffer, 0, area);
    
    update_bodies(false);
    physics_world.Step(0.016, 8, 3);
    update_bodies(true);
}

void Chunk::update_bodies(bool has_ticked)
{
    for(auto& rb : rigidbodies)
    {
        if(rb.body->GetType() == b2_staticBody)
            continue;

        f32 a = rb.body->GetAngle();
        f32 s = std::sin(a);
        f32 c = std::cos(a);
        
        for(u32 i = area; i > 0; --i)
        {
            u32 x = std::floor(i % size);
            u32 y = std::floor(i / size);

            const Cell& cell = rb.cells[i];
            
            if(cell.id == CellType::EMPTY)
                continue;
            
            f32 nx = x * c - y * s;
            f32 ny = x * s + y * c;

            b2Vec2 p = rb.body->GetPosition();
            i32 px = (i32)(p.x + nx) - world_x;
            i32 py = (i32)(p.y + ny) - world_y;

            if(has_ticked)
                set_cell(px, py, cell);
            else
                set_cell(px, py, Cell());
        }
    }
}

void Chunk::update_sand(i32 x, i32 y)
{
    Cell origin = get_cell(x, y);
    
    if(is_empty(x, y + 1))
    {
        Cell target = get_cell(x, y + 1);
        set_cell(x, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x - 1, y + 1))
    {
        Cell target = get_cell(x - 1, y + 1);
        set_cell(x - 1, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x + 1, y + 1))
    {
        Cell target = get_cell(x + 1, y + 1);
        set_cell(x + 1, y + 1, origin);
        set_cell(x, y, target);
    }
}

void Chunk::update_water(i32 x, i32 y)
{
    Cell origin = get_cell(x, y);
    set_updated(x, y, 1);
    
    i8 sign = (rand() % 2) * 2 - 1;

    if(is_empty(x, y + 1))
    {
        Cell target = get_cell(x, y + 1);
        set_cell(x, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x + sign, y + 1))
    {
        Cell target = get_cell(x + sign, y + 1);
        set_cell(x + sign, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x - sign, y + 1))
    {
        Cell target = get_cell(x - sign, y + 1);
        set_cell(x - sign, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x + sign, y))
    {
        Cell target = get_cell(x + sign, y);
        set_cell(x + sign, y, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x - sign, y))
    {
        Cell target = get_cell(x - sign, y);
        set_cell(x - sign, y, origin);
        set_cell(x, y, target);
    }
}
