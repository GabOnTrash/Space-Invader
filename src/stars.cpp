#include "stars.hpp"

void Star::InitStars(int tipo)
{
    for (int i = 0; i < STAR_NUM; i++)
    {
        stars[i].position = { GetRN<float>(0, ViewPort::BASE_WIDTH), GetRN<float>(0, ViewPort::BASE_HEIGHT)};
        if (tipo == 1)
        {
            stars[i].speed.x = GetRN<float>(50, 100) / 10;
            stars[i].speed.y = -GetRN<float>(50, 100) / 10;
        }
        else
        {
            stars[i].speed.x = 0;
            stars[i].speed.y = GetRN<float>(70, 120) / 5;
        }
    }
}
void Star::updateStars(float deltaT, int tipo)
{
    for (int i = 0; i < STAR_NUM; i++)
    {
        stars[i].position.x += stars[i].speed.x * deltaT;
        stars[i].position.y += stars[i].speed.y * deltaT;

        if (tipo == 1)
        {
            if (stars[i].position.x > ViewPort::BASE_WIDTH + 10)
                stars[i].position.x = GetRN<float>(-50, 0);

            if (stars[i].position.y < -10)
                stars[i].position.y = GetRN<float>(ViewPort::BASE_HEIGHT, ViewPort::BASE_HEIGHT + 50);
        }
        else
        {
            if (stars[i].position.y > ViewPort::BASE_HEIGHT + 10)
            {
                stars[i].position.y = GetRN<float>(-50.0f, 0.0f);
                stars[i].position.x = GetRN<float>(3, ViewPort::BASE_WIDTH - 3);
            }       
        }
    }
}
void Star::DrawStars()
{
    for (int i = 0; i < STAR_NUM; i++)
    {
        DrawCircleV(stars[i].position, radius, WHITE);
    }
}