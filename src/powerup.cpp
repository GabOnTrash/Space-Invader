#include "powerup.hpp"

PowerUp::PowerUp(int tipo)
{
    posizione.x = GetRN<float>(0, BASE_WIDTH);
    posizione.y = GetRN<float>(-150, -100);
    
    switch (tipo)
    {
    case 0:
        tipoPotenziamento = "downgrade";
        break;
    case 1:
        tipoPotenziamento = "depotCuore";
        break;
    case 2:
        tipoPotenziamento = "triploLaser";
        break;
    case 3:
        tipoPotenziamento = "potCuore";
        break;
    case 4:
        tipoPotenziamento = "bigLaser";
        break;
    default:
        tipoPotenziamento = "";
    }

    immagine = &arrayPotenziamenti[tipo];
}
PowerUp::~PowerUp()
{
}
Rectangle PowerUp::getBounds()
{
    return { posizione.x, posizione.y, static_cast<float>(immagine->width), static_cast<float>(immagine->height) };
}
void PowerUp::Init()
{
    if (loadedResources)
    {
        arrayPotenziamenti[0] = AssetsManager::GetTexture("reduced_vel");
        arrayPotenziamenti[1] = AssetsManager::GetTexture("minus_one_heart");
        arrayPotenziamenti[2] = AssetsManager::GetTexture("triple_laser");
        arrayPotenziamenti[3] = AssetsManager::GetTexture("plus_one_heart");
        arrayPotenziamenti[4] = AssetsManager::GetTexture("_big_laser");

        byteMask.loadFromImage(PATH_BYTEMASK_MODIFIERS);

        audio = AssetsManager::GetSound("modifier_sound");
    }
}

void PowerUp::Disegna()
{
    DrawTexture(*immagine, posizione.x, posizione.y, WHITE);
}
void PowerUp::Movimento(float deltaT)
{
    posizione.y += vel * deltaT;
}
void PowerUp::Aggiorna(float deltaT)
{
    Movimento(deltaT);
}