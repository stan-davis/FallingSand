#include "input.h"
#include <memory>

Input::Input()
{
    keyboard_state = SDL_GetKeyboardState(&key_length);
    prev_key_state = new u8[key_length];
    memcpy(prev_key_state, keyboard_state, key_length);
}

Input::~Input()
{
    delete[] prev_key_state;
    prev_key_state = nullptr;
}

void Input::update()
{
    mouse_state = SDL_GetMouseState(&mouse_position_x, &mouse_position_y);
}

void Input::late_update()
{
    memcpy(prev_key_state, keyboard_state, key_length);
    prev_mouse_state = mouse_state;
}

bool Input::is_key_pressed(SDL_Scancode scancode)
{
    return !prev_key_state[scancode] && keyboard_state[scancode];
}

bool Input::is_key_held(SDL_Scancode scancode)
{
    return keyboard_state[scancode];
}

bool Input::is_key_released(SDL_Scancode scancode)
{
    return prev_key_state[scancode] && !keyboard_state[scancode];
}

bool Input::is_mouse_pressed(MouseState state)
{
    u32 mask = get_mouse_state_mask(state);
    return !(prev_mouse_state & mask) && (mouse_state & mask);
}

bool Input::is_mouse_held(MouseState state)
{
    return (mouse_state & get_mouse_state_mask(state));
}

bool Input::is_mouse_released(MouseState state)
{
    u32 mask = get_mouse_state_mask(state);
    return (prev_mouse_state & mask) && !(mouse_state & mask);
}

u32 Input::get_mouse_state_mask(MouseState state)
{
    u32 mask = 0;

    switch (state)
    {
    case BUTTON_LEFT:
        mask = SDL_BUTTON_LMASK;
        break;
    case BUTTON_RIGHT:
        mask = SDL_BUTTON_RMASK;
        break;
    case BUTTON_MIDDLE:
        mask = SDL_BUTTON_MMASK; 
        break;
    case BUTTON_WHEEL_DOWN:
        mask = SDL_BUTTON_X1MASK;
        break;
    case BUTTON_WHEEL_UP:
        mask = SDL_BUTTON_X2MASK;
        break;
    }

    return mask;
}
