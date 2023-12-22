#pragma once

#include <box2d.h>
#include <vector>
#include "cell.h"

class Rigidbody
{
    public:
        Rigidbody(std::vector<Cell> _cells, b2BodyType type, b2World& world);
        ~Rigidbody() = default;
        
        b2Body* body = nullptr;
        std::vector<Cell> cells;
};