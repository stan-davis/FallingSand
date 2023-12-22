#pragma once

#include "types.h"

///Time Singleton
class Time
{   
public:
    Time(u32 _frame_rate);
    ~Time() = default;

    static void update();
    static void late_update();
    
    static f32 get_delta_time() { return singleton->dt; }
    static u32 get_frames_per_second() { return singleton->frame_rate; }

private:
    static Time *singleton;

    f32 dt;
    f32 now;
    f32 last;

    f32 frame_last;
    f32 frame_delay;
    f32 frame_time;

    u32 frame_rate;
    u32 frame_count;
};