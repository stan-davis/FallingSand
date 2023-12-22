#pragma once
#include "types.h"

typedef struct Color
{
    u8 r = 0, g = 0, b = 0;
    u32 to_uint() { return (r << 0) + (g << 8) + (b << 16) + (255 << 24); };
} Color;

typedef enum CellType
{
    EMPTY,
    SAND,
    WATER,
    STONE,
    GRASS,
    DIRT,
} CellType;

typedef struct Cell
{
    u8 id = 0;
    Color color = {0, 0, 0};
    f32 vx = 0, vy = 0;

} Cell;
