#include "stelle.hpp"

void Stella::InizializzaStelle(int tipo)
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        stelle[i].posizione = { GetRN<float>(0, WINDOW_WIDTH), GetRN<float>(0, WINDOW_HEIGHT)};
        if (tipo == 1)
        {
            stelle[i].velocita.x = GetRN<float>(50, 100) / 10 * SCALE;
            stelle[i].velocita.y = -GetRN<float>(50, 100) / 10 * SCALE;
        }
        else
        {
            stelle[i].velocita.x = 0 * SCALE;
            stelle[i].velocita.y = GetRN<float>(70, 120) / 5 * SCALE;
        }
    }
}
void Stella::AggiornaStelle(float deltaT, int tipo)
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        stelle[i].posizione.x += stelle[i].velocita.x * deltaT;
        stelle[i].posizione.y += stelle[i].velocita.y * deltaT;

        if (tipo == 1)
        {
            if (stelle[i].posizione.x > WINDOW_WIDTH + 10)
                stelle[i].posizione.x = GetRN<float>(-50, 0);

            if (stelle[i].posizione.y < -10)
                stelle[i].posizione.y = GetRN<float>(WINDOW_HEIGHT, WINDOW_HEIGHT + 50);
        }
        else
        {
            if (stelle[i].posizione.y > WINDOW_HEIGHT + 10)
            {
                stelle[i].posizione.y = GetRN<float>(-50.0f, 0.0f);
                stelle[i].posizione.x = GetRN<float>(3, WINDOW_WIDTH - 3);
            }       
        }
    }
}
void Stella::DisegnaStelle()
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        DrawCircleV(stelle[i].posizione, radius, WHITE);
    }
}