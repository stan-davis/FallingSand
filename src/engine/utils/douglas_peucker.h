#pragma once

#include <vector>
#include <math.h>
#include "smath.h"

///adapted from https://github.com/GiWanPark/DouglasPeucker-Algorithm/blob/master/DouglasPeucker.h
namespace DouglasPeucker
{
    f32 perpendicular_distance(const vec2& point, const vec2& start, const vec2& end)
    {
        f32 dx = end.x - start.x;
        f32 dy = end.y - start.y;

        f32 mag = std::sqrt((dx * dx) + (dy * dy));
        if(mag > 0.0)
        {
            dx /= mag;
            dy /= mag;
        }

        f32 pvx = point.x - start.x;
        f32 pvy = point.y - start.y;
        
        f32 pvdot = dx * pvx + dy * pvy;

        f32 dsx = pvdot * dx;
        f32 dsy = pvdot * dy;

        f32 ax = pvx - dsx;
        f32 ay = pvy - dsy;

        return std::sqrt((ax * ax) + (ay * ay));
    }

    std::vector<vec2> simplify(const std::vector<vec2>& points, f32 threshold)
    {
        if(points.size() < 2)
            return points;

        f32 max_distance = 0.0;
        u32 index = 0;
        u32 end = points.size() - 1;

        for(u32 i = 0; i < end; i++)
        {
            f32 d = perpendicular_distance(points[i], points[0], points[end]);
            
            if(d > max_distance)
            {
                index = i;
                max_distance = d;
            }
        }

        std::vector<vec2> result;

        //recursively simplify
        if(max_distance > threshold)
        {
            std::vector<vec2> first_line(points.begin(), points.begin() + index + 1);
            std::vector<vec2> last_line(points.begin() + index, points.end());
            std::vector<vec2> results1 = simplify(first_line, threshold);
            std::vector<vec2> results2 = simplify(last_line, threshold);
            
            //build results
            result.assign(results1.begin(), results1.end() - 1);
            result.insert(result.end(), results2.begin(), results2.end());
        }
        else
        {
            result.push_back(points[0]);
            result.push_back(points[end]);
        }

        return result;
    };
}