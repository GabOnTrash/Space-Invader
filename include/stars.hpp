#pragma once

#include "specs.hpp"

#define STAR_NUM 50 * (1500 / 1000)

struct AttribStar
{
    Vector2 position;
    Vector2 speed;
};

class Star
{
public:

    AttribStar stars[STAR_NUM] = { 0, 0 };

    void InitStars(int tipo);
    void updateStars(float deltaT, int tipo);
    void DrawStars();

private:
    float radius = 4;
};