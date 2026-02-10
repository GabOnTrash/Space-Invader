#pragma once

#include "../util/specs.hpp"

#define STARS_NUM 300

struct AttribStar
{
    Vector2 position;
    Vector2 speed;
};

class Star
{
public:
    Star(GameContext& ctx);
    void updateStars(float deltaT);
    void DrawStars();

    AttribStar stars[STARS_NUM] = { 0, 0 };

private:
    const float radius = 3.f;
    GameContext& ctx;
};