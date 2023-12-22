#include "input.h"
#include <memory>

Input* Input::singleton = new Input();

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
    singleton = nullptr;
}

void Input::update()
{
    int px, py;
    singleton->mouse_state = SDL_GetMouseState(&px, &py);
    singleton->mouse_position = {(f32)px, (f32)py};
}

void Input::late_update()
{
    memcpy(singleton->prev_key_state, singleton->keyboard_state, singleton->key_length);
    singleton->prev_mouse_state = singleton->mouse_state;
}

bool Input::is_key_pressed(SDL_Scancode scancode)
{
    return !singleton->prev_key_state[scancode] && singleton->keyboard_state[scancode];
}

bool Input::is_key_held(SDL_Scancode scancode)
{
    return singleton->keyboard_state[scancode];
}

bool Input::is_key_released(SDL_Scancode scancode)
{
    return singleton->prev_key_state[scancode] && !singleton->keyboard_state[scancode];
}

bool Input::is_mouse_pressed(MouseState state)
{
    u32 mask = singleton->get_mouse_state_mask(state);
    return !(singleton->prev_mouse_state & mask) && (singleton->mouse_state & mask);
}

bool Input::is_mouse_held(MouseState state)
{
    return (singleton->mouse_state & singleton->get_mouse_state_mask(state));
}

bool Input::is_mouse_released(MouseState state)
{
    u32 mask = singleton->get_mouse_state_mask(state);
    return (singleton->prev_mouse_state & mask) && !(singleton->mouse_state & mask);
}

vec2 Input::get_mouse_position()
{
    return singleton->mouse_position;
}

u32 Input::get_mouse_state_mask(MouseState state) const
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
