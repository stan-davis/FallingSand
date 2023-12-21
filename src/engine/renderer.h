#pragma once

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL.h>

#include "utils/types.h"

class Renderer
{
    public:
        Renderer(u16 _window_width, u16 _window_height, u8 _resolution_scale);
        ~Renderer();
        
        void start_frame();
        void render();
        void end_frame();
        
        void draw(int x, int y, u32 color);
        void draw_cursor(int x, int y, int brush_size);
        
        SDL_Renderer* get_renderer() { return renderer; };
        SDL_Window* get_window() { return window; };
        u8 get_resolution_scale() { return resolution_scale; };
        
    private:
        void clear_buffer();

        SDL_Renderer* renderer;
        SDL_Window* window;

        u16 window_width;
        u16 window_height;
        
        u8 resolution_scale;
        u16 scaled_width;
        u16 scaled_height;
        
        SDL_Texture* frame_texture;
        u32* frame_buffer;
};