#pragma once

#include "specs.hpp"

#define STARS_NUM 300

struct AttribStar
{
    Vector2 position;
    Vector2 speed;
};

class Star
{
public:

    AttribStar stars[STARS_NUM] = { 0, 0 };

    void InitStars(int tipo);
    void updateStars(float deltaT, int tipo);
    void DrawStars();

private:
    float radius = 3;
};