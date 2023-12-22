#pragma once

#include "smath.h"
#include <SDL.h>

///Input Singleton
class Input
{
public:
	enum MouseState 
	{
		BUTTON_LEFT, 
		BUTTON_RIGHT, 
		BUTTON_MIDDLE, 
		BUTTON_WHEEL_UP, 
		BUTTON_WHEEL_DOWN
	};

	Input();
	~Input();

	//Update
	static void update();
	static void late_update();

	//Accessors
	static bool is_key_pressed(SDL_Scancode scancode);
	static bool is_key_held(SDL_Scancode scancode);
	static bool is_key_released(SDL_Scancode scancode);

	static bool is_mouse_pressed(MouseState state);
	static bool is_mouse_held(MouseState state);
	static bool is_mouse_released(MouseState state);
	
	static vec2 get_mouse_position();
private:
	static Input *singleton;

	//Keyboard
	const u8 *keyboard_state;
	u8* prev_key_state;
	i32 key_length;
    
	//Mouse
	u32 get_mouse_state_mask(MouseState state) const;
	u32 mouse_state;
	u32 prev_mouse_state;
	vec2 mouse_position;
};

