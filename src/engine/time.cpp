#include "time.h"
#include <SDL.h>

Time::Time(u32 _frame_rate)
{
    frame_rate = _frame_rate;
    frame_delay = 1000.0 / frame_rate;
}

void Time::update()
{
    now = static_cast<f32>(SDL_GetTicks());
    dt = (now - last) / 1000.0;
    last = now;
    ++frame_count;

    if(now - frame_last >= 1000.0)
    {
        frame_rate = frame_count;
        frame_count = 0;
        frame_last = now;
    }
}

void Time::late_update()
{
    frame_time = static_cast<f32>(SDL_GetTicks()) - now;
    if(frame_delay > frame_time)
    {
        SDL_Delay(frame_delay - frame_time);
    }
}