#include "powerup.hpp"

PowerUp::PowerUp(int tipo)
{
    posizione.x = GetRN<float>(0, WINDOW_WIDTH);
    posizione.y = GetRN<float>(-150, -100);

    vel = 200 * SCALE;
    
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
    return { posizione.x, posizione.y, static_cast<float>(immagine->width) * SCALE, static_cast<float>(immagine->height) * SCALE };
}
void PowerUp::Init()
{
    if (!textureCaricata)
    {
        arrayPotenziamenti[0] = LoadTexture("immagini/downgrade.png");
        arrayPotenziamenti[1] = LoadTexture("immagini/depotenziamento_cuore.png");
        arrayPotenziamenti[2] = LoadTexture("immagini/triplo_laser.png");
        arrayPotenziamenti[3] = LoadTexture("immagini/potenziamento_cuore.png");
        arrayPotenziamenti[4] = LoadTexture("immagini/sparo_veloce.png");

        audio = LoadSound("immagini/power_up_sound.mp3");
        textureCaricata = true;
    }
}
void PowerUp::Unload()
{
    if (textureCaricata)
    {
        for (int i = 0; i < 5; i++)
            UnloadTexture(arrayPotenziamenti[i]);
        
        UnloadSound(audio);
        textureCaricata = false;
    }
}
void PowerUp::Disegna()
{
    DrawTextureEx(*immagine, posizione, 0, SCALE, WHITE);
}
void PowerUp::Movimento(float deltaT)
{
    posizione.y += vel * deltaT;
}
void PowerUp::Aggiorna(float deltaT)
{
    Movimento(deltaT);
}