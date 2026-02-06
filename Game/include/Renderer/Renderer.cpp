#include "Renderer.hpp"

Renderer::Renderer()
{
}
Renderer::~Renderer()
{
}

void Renderer::Init()
{
    //SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(BASE_WIDTH, BASE_HEIGHT,/*GetMonitorWidth(0), GetMonitorHeight(0),*/ "Space Invaders");
    target = LoadRenderTexture(BASE_WIDTH, BASE_HEIGHT);
    UpdateScaling();
}
void Renderer::UpdateScaling()
{
    scale = std::min((float)GetScreenWidth() / BASE_WIDTH, (float)GetScreenHeight() / BASE_HEIGHT);
    offset.x = (GetScreenWidth() - BASE_WIDTH * scale) * 0.5f;
    offset.y = (GetScreenHeight() - BASE_HEIGHT * scale) * 0.5f;
}
void Renderer::Begin()
{
    BeginTextureMode(target);
}
void Renderer::EndAndDraw()
{
    EndTextureMode();
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(target.texture,
            {0, 0, (float)BASE_WIDTH, -(float)BASE_HEIGHT},
            {offset.x, offset.y, BASE_WIDTH * scale, BASE_HEIGHT * scale},
            {0, 0}, 0.0f, WHITE);
    EndDrawing();
}
Vector2 Renderer::GetVirtualMouse()
{
    Vector2 mouse = GetMousePosition();
    return { (mouse.x - offset.x) / scale, (mouse.y - offset.y) / scale };
}
