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

    const int BASE_WIDTH = 3840;
    const int BASE_HEIGHT = 2160;
    float scale;
    Vector2 offset;

private:
    RenderTexture2D target;
};
