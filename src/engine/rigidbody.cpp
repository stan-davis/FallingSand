#include "rigidbody.h"

#include <MarchingSquares.h>
#include "utils/douglas_peucker.h"
#include <polypartition.h>

Rigidbody::Rigidbody(Cell* _cells, int size, b2World& world)
{
    //Init
    const int area = size * size;
    cells = new Cell[area];
    cells = _cells;
    
    unsigned char* data = new unsigned char[area];
    memset(data, 0, area * sizeof(unsigned char));

    for(int i = 0; i < area; i++)
    {
        if(cells[i].id == CellType::STONE)
            data[i] = 1;
    }

    //Marching squares
    std::vector<vec2> directions;
    MarchingSquares::Result r = MarchingSquares::FindPerimeter(size, size, data);
    
    vec2 last = {(f32)r.initialX, (f32)r.initialY};

    for(u32 i = 0; i < r.directions.size(); i++) 
    {
        for(u32 ix = 0; ix < std::max(abs(r.directions[i].x), 1); ix++) {
            for(u32 iy = 0; iy < std::max(abs(r.directions[i].y), 1); iy++) 
            {
                u32 ilx = (u32)(last.x + ix * (r.directions[i].x < 0 ? -1 : 1));
                u32 ily = (u32)(last.y - iy * (r.directions[i].y < 0 ? -1 : 1));
                
                if(ilx < 0) ilx = 0;
                if(ilx >= size) ilx = size - 1;

                if(ily < 0) ily = 0;
                if(ily >= size) ily = size - 1;

                u32 ind = ilx + ily * size;
                if(ind >= size) 
                {
                    continue;
                }
            }
        }

        last.x += (f32)r.directions[i].x;
        last.y -= (f32)r.directions[i].y;
        directions.push_back(last);
    }

    //Simplify polygons
    directions = DouglasPeucker::simplify(directions, 1.0);
    
    //Trianglulate
    TPPLPoly poly;
    poly.Init((long)directions.size());

    for(i32 i = 0; i < directions.size(); i++)
        poly[directions.size() - i - 1] = {directions[i].x, directions[i].y};

    if(poly.GetOrientation() == TPPL_ORIENTATION_CW)
        poly.SetHole(true);
    
    if(poly.GetNumPoints() < 3)
        return;
    
    //Create triangles
    TPPLPartition part;
    std::list<TPPLPoly> result;
    part.Triangulate_EC(&poly, &result);

    //Create physics body
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(0.0, 0.0);
    body = world.CreateBody(&def);
    
    for(auto& p : result)
    {
        if((p[0].x == p[1].x && p[1].x == p[2].x) || (p[0].y == p[1].y && p[1].y == p[2].y))
            continue;
        
        b2Vec2 vec[3] = 
        {
            {(float)p[0].x, (float)p[0].y},
            {(float)p[1].x, (float)p[1].y},
            {(float)p[2].x, (float)p[2].y}
        };

        b2PolygonShape shape;
        shape.Set(vec, 3);

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = 1.0;
        fixture.friction = 0.3;
        
        body->CreateFixture(&fixture);
    }
};
