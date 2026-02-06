#include "stars.hpp"

Star::Star()
{
    for (int i = 0; i < STARS_NUM; i++)
    {
        stars[i].position = { GetRN<float>(0.0f, static_cast<float>(ViewPort::BASE_WIDTH)), GetRN<float>(0.0f, static_cast<float>(ViewPort::BASE_HEIGHT))};

        stars[i].speed.x = GetRN<float>(-0.2f, 0.2f);
        stars[i].speed.y = GetRN<float>(70.f, 120.f) / 5;
    }
}
void Star::updateStars(float deltaT)
{
    for (int i = 0; i < STARS_NUM; i++)
    {
        stars[i].position.x += stars[i].speed.x * deltaT;
        stars[i].position.y += stars[i].speed.y * deltaT;

        if (stars[i].position.y > ViewPort::BASE_HEIGHT + 10)
        {
            stars[i].position.y = GetRN<float>(-50.0f, 0.0f);
            stars[i].position.x = GetRN<float>(3.f, static_cast<float>(ViewPort::BASE_WIDTH - 3));
        }
    }
}
void Star::DrawStars()
{
    for (int i = 0; i < STARS_NUM; i++)
    {
        DrawCircleV(stars[i].position, radius, WHITE);
    }
}