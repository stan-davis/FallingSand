#include "world.h"
#include <cstdio>

#include "utils/smath.h"
#include <FastNoiseLite.h>

World::World(Renderer* _renderer)
{
    debug_draw.SetRenderer(_renderer);
    debug_draw.SetFlags(b2Draw::e_shapeBit);
    physics_world.SetDebugDraw(&debug_draw);
    
    //Ground body
    b2BodyDef def;
    def.type = b2_staticBody;
    def.position.Set(0.0, 121.0);
    b2Body* body = physics_world.CreateBody(&def);

    b2PolygonShape shape;
    shape.SetAsBox(120,1);

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0;
    fixture.friction = 0.3;

    body->CreateFixture(&fixture);

    //Terrain
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    for(u16 y = 0; y < size; y++)
        for(u16 x = 0; x < size; x++)
        {
            u16 amplitude = 10.0;
            u16 r = rand() % 4 + 1;
            i32 n1 = std::floor((noise.GetNoise((f32)x, (f32)y) + (amplitude * 0.5)) * amplitude);
            i32 n2 = std::floor((noise.GetNoise((f32)x + 0.01, (f32)y + 0.01) + (amplitude * 0.5)) * amplitude);
            i32 n = n1 + n2;

            if(y > n)
                create_cell(x, y, CellType::GRASS);
            if(y > n + r)
                create_cell(x, y, CellType::DIRT);
        }

    apply_draw();
}

void World::set_cell(u16 x, u16 y, Cell cell)
{
    if(!in_bounds(x,y))
        return;
    
    cells[y * size + x] = cell;
}

void World::create_cell(u16 x, u16 y, u8 id)
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

Cell World::get_cell(u16 x, u16 y)
{
    return cells[y * size + x];
}

bool World::is_empty(u16 x, u16 y)
{
    return cells[y * size + x].id == 0;
}

bool World::in_bounds(u16 x, u16 y)
{
    return x >= 0 && x < size && y >= 0 && y < size;
}

void World::set_updated(u16 x, u16 y, u8 value)
{
    update_buffer[y * size + x] = value;
}

bool World::is_updated(u16 x, u16 y)
{
    return update_buffer[y * size + x] > 0;
}

void World::apply_draw()
{
    if(draw_canvas.empty())
        return;

    //Create rigidbodies
    Cell* arr = new Cell[area];
    bool found = false;

    for(u16 i = 0; i < area; i++)
    {
        if(draw_canvas[i].id == CellType::STONE)
        {
            arr[i] = draw_canvas[i];
            found = true;
        }
    }

    if(found)
    {
        Rigidbody rb = Rigidbody(arr, size, physics_world);
        rigidbodies.push_back(rb);
    }

    //Apply non-empty cells
    for(u16 i = 0; i < area; i++)
    {
        if(draw_canvas[i].id != CellType::EMPTY)
            cells[i] = draw_canvas[i];
    }
    
    //Clear draw canvas
    for(auto& cell : draw_canvas)
        cell = Cell();
}

void World::render(Renderer *renderer)
{
    for(u16 y = size - 1; y > 0; --y)
        for(u16 x = size - 1; x > 0; --x)
        {
            Cell buffer_cell = get_cell(x, y);
            Cell canvas_cell = draw_canvas[y * size + x];
            u32 c = buffer_cell.color.to_uint();

            if(canvas_cell.id != CellType::EMPTY)
                c = canvas_cell.color.to_uint();

            renderer->draw(x, y, c);
        }
}

void World::render_debug()
{
    physics_world.DebugDraw();
}

void World::update_bodies(bool has_ticked)
{
    for(auto& rb : rigidbodies)
    {
        f32 a = rb.body->GetAngle();
        f32 s = std::sin(a);
        f32 c = std::cos(a);

        for(u16 y = size - 1; y > 0; --y)
            for(u16 x = size - 1; x > 0; --x)
            {
                const Cell& cell = rb.cells[y * size + x];
                
                if(cell.id == CellType::EMPTY)
                    continue;
                
                f32 nx = x * c - y * s;
                f32 ny = x * s + y * c;

                b2Vec2 p = rb.body->GetPosition();
                u16 px = (u16)(p.x + nx);
                u16 py = (u16)(p.y + ny);

                if(has_ticked)
                    set_cell(px, py, cell);
                else
                    set_cell(px, py, Cell());
            }
    }
}

void World::update(f32 dt)
{
    for(u16 y = size - 1; y > 0; --y)
        for(u16 x = size - 1; x > 0; --x)
        {
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

void World::update_sand(u16 x, u16 y)
{
    Cell origin = get_cell(x, y);

    if(is_empty(x, y + 1) && in_bounds(x, y + 1))
    {
        Cell target = get_cell(x, y + 1);
        set_cell(x, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x - 1, y + 1) && in_bounds(x - 1, y + 1))
    {
        Cell target = get_cell(x - 1, y + 1);
        set_cell(x - 1, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x + 1, y + 1) && in_bounds(x + 1, y + 1))
    {
        Cell target = get_cell(x + 1, y + 1);
        set_cell(x + 1, y + 1, origin);
        set_cell(x, y, target);
    }
}

void World::update_water(u16 x, u16 y)
{
    Cell origin = get_cell(x, y);
    set_updated(x, y, 1);
    
    i8 sign = (rand() % 2) * 2 - 1;

    if(is_empty(x, y + 1) && in_bounds(x, y + 1))
    {
        Cell target = get_cell(x, y + 1);
        set_cell(x, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x + sign, y + 1) && in_bounds(x + sign, y + 1))
    {
        Cell target = get_cell(x + sign, y + 1);
        set_cell(x + sign, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x - sign, y + 1) && in_bounds(x - sign, y + 1))
    {
        Cell target = get_cell(x - sign, y + 1);
        set_cell(x - sign, y + 1, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x + sign, y) && in_bounds(x + sign, y))
    {
        Cell target = get_cell(x + sign, y);
        set_cell(x + sign, y, origin);
        set_cell(x, y, target);
    }
    else if(is_empty(x - sign, y) && in_bounds(x - sign, y))
    {
        Cell target = get_cell(x - sign, y);
        set_cell(x - sign, y, origin);
        set_cell(x, y, target);
    }
}
