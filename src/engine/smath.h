#pragma once

#include "types.h"

#include <tuple>
#include <functional>

typedef struct vec2
{
    f32 x, y;
} vec2;

typedef struct vec2i
{
    i32 x, y;

    vec2i operator+(const vec2i& v) const { return {x + v.x, y + v.y}; };
	vec2i operator-(const vec2i& v) const { return {x - v.x, y - v.y}; };
	vec2i operator*(const vec2i& v) const { return {x - v.x, y - v.y}; };
	void operator+=(const vec2i& v) { x += v.x; y += v.y; };
	void operator-=(const vec2i& v) { x -= v.x; y -= v.y; };
	void operator*=(const vec2i& v) { x *= v.x; y *= v.y; };
	bool operator==(const vec2i& v) const { return x == v.x && y == v.y; };
	bool operator!=(const vec2i& v) const { return x != v.x || y != v.y; };
} vec2i;

struct pair_hash
{
	size_t operator()(const vec2i &v) const
	{
		return std::hash<i32>()((v.x * 73856093) ^ (v.y * 83492791));
	}
};
