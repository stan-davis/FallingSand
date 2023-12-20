#pragma once

#include <box2d.h>
#include "../renderer.h"

class DebugDraw : public b2Draw
{
    public:
        ~DebugDraw() {};
        
        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
        {
            SDL_SetRenderDrawColor(renderer->get_renderer(), (color.r * 255), (color.g * 255), (color.b * 255), (color.a * 255));

            for(int i = 0; i < vertexCount; i++)
            {
                int j = (i + 1) % vertexCount;
                b2Vec2 v0 = (float)resolution * vertices[i];
                b2Vec2 v1 = (float)resolution * vertices[j];

                SDL_RenderDrawLine(renderer->get_renderer(), (int)v0.x, (int)v0.y, (int)v1.x, (int)v1.y);
            }
        };

        void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
        {
            SDL_SetRenderDrawColor(renderer->get_renderer(), (color.r * 255), (color.g * 255), (color.b * 255), (color.a * 255));

            for(int i = 0; i < vertexCount; i++)
            {
                int j = (i + 1) % vertexCount;
                b2Vec2 v0 = (float)resolution * vertices[i];
                b2Vec2 v1 = (float)resolution * vertices[j];

                SDL_RenderDrawLine(renderer->get_renderer(), (int)v0.x, (int)v0.y, (int)v1.x, (int)v1.y);
            }
        };

        void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) 
        {

        };

        void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) 
        {

        };

        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) 
        {
            SDL_SetRenderDrawColor(renderer->get_renderer(), (color.r * 255), (color.g * 255), (color.b * 255), (color.a * 255));

            b2Vec2 n1 = (float)resolution * p1;
            b2Vec2 n2 = (float)resolution * p2;

            SDL_RenderDrawLine(renderer->get_renderer(), (int)n1.x, (int)n1.y, (int)n2.x, (int)n1.y);
        };

        void DrawTransform(const b2Transform& xf) 
        {
            
        };

        void DrawPoint(const b2Vec2& p, float size, const b2Color &color) 
        {
            SDL_SetRenderDrawColor(renderer->get_renderer(), (color.r * 255), (color.g * 255), (color.b * 255), (color.a * 255));
            SDL_Rect rect = {resolution * (int)p.x, resolution * (int)p.y, (int)size, (int)size};
            SDL_RenderFillRect(renderer->get_renderer(), &rect);
        };

        void SetRenderer(Renderer* _renderer)
        {
            renderer = _renderer;
            resolution = renderer->get_resolution_scale();
        }
    
    private:
        Renderer* renderer;
        u8 resolution = 0;
};