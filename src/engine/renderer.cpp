#include "renderer.h"
#include <math.h>

Renderer::Renderer(u16 _window_width, u16 _window_height, u8 _resolution_scale)
{
    //Init SDL
    resolution_scale = _resolution_scale;
    window_width = _window_width;
    window_height = _window_height;

    scaled_width = SDL_floor(window_width / resolution_scale);
    scaled_height = SDL_floor(window_height / resolution_scale);

    window = SDL_CreateWindow("SDL Test Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    //Setup framebuffer
    frame_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, scaled_width, scaled_height);
	frame_buffer = new u32[scaled_width * scaled_height];
	clear_buffer();
}

Renderer::~Renderer()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Renderer::start_frame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::render()
{
	SDL_UpdateTexture(frame_texture, nullptr, frame_buffer, scaled_width * sizeof(u32));
	SDL_RenderCopy(renderer, frame_texture, NULL, NULL);
    clear_buffer();
}

void Renderer::end_frame()
{
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

void Renderer::draw(int x, int y, u32 color)
{
    if (x >= 0 && x < scaled_width && y >= 0 && y < scaled_height)
    {
        frame_buffer[y * scaled_width + x] = color;
    }
}

void Renderer::draw_cursor(int x, int y, int brush_size)
{
    int b = brush_size * resolution_scale;
    int nbx = x - b;
    int nby = y - b;
    int pbx = b * 2;
    int pby = b * 2;

    SDL_Rect r = {nbx, nby, pbx, pby};
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &r);
}

void Renderer::clear_buffer()
{
    memset(frame_buffer, 0, scaled_width * scaled_height * sizeof(u32));
}