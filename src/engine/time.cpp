#include "time.h"
#include <SDL.h>

Time* Time::singleton = new Time(75);

Time::Time(u32 _frame_rate)
{
    frame_rate = _frame_rate;
    frame_delay = 1000.0 / frame_rate;
}

void Time::update()
{
    singleton->now = static_cast<f32>(SDL_GetTicks());
    singleton->dt = (singleton->now - singleton->last) / 1000.0;
    singleton->last = singleton->now;
    ++singleton->frame_count;

    if(singleton->now - singleton->frame_last >= 1000.0)
    {
        singleton->frame_rate = singleton->frame_count;
        singleton->frame_count = 0;
        singleton->frame_last = singleton->now;
    }
}

void Time::late_update()
{
    singleton->frame_time = static_cast<f32>(SDL_GetTicks()) - singleton->now;
    if(singleton->frame_delay > singleton->frame_time)
    {
        SDL_Delay(singleton->frame_delay - singleton->frame_time);
    }
}