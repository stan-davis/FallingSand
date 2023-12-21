#pragma once

#include "utils/types.h"
#include <SDL.h>

typedef enum MouseState 
{
    BUTTON_LEFT, 
    BUTTON_RIGHT, 
    BUTTON_MIDDLE, 
    BUTTON_WHEEL_UP, 
    BUTTON_WHEEL_DOWN
} MouseState;

class Input
{
public:
	Input();
	~Input();

	//Update
	void update();
	void late_update();

	//Input polling
	bool is_key_pressed(SDL_Scancode scancode);
	bool is_key_held(SDL_Scancode scancode);
	bool is_key_released(SDL_Scancode scancode);

	bool is_mouse_pressed(MouseState state);
	bool is_mouse_held(MouseState state);
	bool is_mouse_released(MouseState state);

	i32 get_mouse_x() { return mouse_position_x; };
	i32 get_mouse_y() { return mouse_position_y; };
private:
	u32 get_mouse_state_mask(MouseState state);

	//Keyboard
	const u8* keyboard_state;
	u8* prev_key_state;
	i32 key_length;
    
	//Mouse
	u32 mouse_state;
	u32 prev_mouse_state;
    i32 mouse_position_x;
    i32 mouse_position_y;
};

