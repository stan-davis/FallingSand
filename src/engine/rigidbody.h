#pragma once

#include <box2d.h>
#include <vector>
#include "cell.h"

class Rigidbody
{
    public:
        Rigidbody(Cell* _cells, int size, b2World& world);
        ~Rigidbody() = default;
        
        b2Body* body = nullptr;
        Cell* cells;
};