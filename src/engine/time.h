#pragma once

#include "utils/types.h"

class Time
{   
    public:
        Time(u32 _frame_rate);
        ~Time() = default;

        void update();
        void late_update();
        
        f32 get_delta_time() { return dt; }
        u32 get_frames_per_second() { return frame_rate; }
    
    private:
        f32 dt;
        f32 now;
        f32 last;

        f32 frame_last;
        f32 frame_delay;
        f32 frame_time;

        u32 frame_rate;
        u32 frame_count;
};