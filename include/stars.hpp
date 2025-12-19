#pragma once

#include "specs.hpp"

#define NUM_STELLE 50 * (1500 / 1000)

struct AttribStelle
{
    Vector2 posizione;
    Vector2 velocita;
};

class Stella
{
public:

    AttribStelle stelle[NUM_STELLE] = { 0, 0 };

    void InizializzaStelle(int tipo);
    void AggiornaStelle(float deltaT, int tipo);
    void DisegnaStelle();

    float radius = 3;
};