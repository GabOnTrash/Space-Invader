#pragma once
#include "raylib.h"
#include <math.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Init();
    void UpdateScaling();
    void Begin();
    void EndAndDraw();
    Vector2 GetVirtualMouse();

    const int BASE_WIDTH = 1280;
    const int BASE_HEIGHT = 720;

private:
    RenderTexture2D target;
    float scale;
    Vector2 offset;
};
