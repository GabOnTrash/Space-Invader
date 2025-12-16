#include "cuore.hpp"

Rectangle Cuore::getBounds()
{
    return { posizione.x * SCALE, posizione.y * SCALE, static_cast<float>(immagine.width) * SCALE, static_cast<float>(immagine.height) * SCALE };
}
void Cuore::Init()
{
    if (!textureCaricata)
    {
        immagine = LoadTexture("immagini/cuore.png");
        textureCaricata = true;
    }
}
void Cuore::Unload()
{
    if (textureCaricata)
    {
        UnloadTexture(immagine);
        textureCaricata = false;
    }
}
void Cuore::Disegna()
{
    DrawTextureEx(Cuore::immagine, posizione, 0, SCALE * MULTIPLY, WHITE);
}
