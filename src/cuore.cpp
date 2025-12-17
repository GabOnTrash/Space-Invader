#include "cuore.hpp"

Rectangle Cuore::getBounds()
{
    return { posizione.x, posizione.y, static_cast<float>(immagine.width), static_cast<float>(immagine.height) };
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
    DrawTexture(Cuore::immagine, posizione.x, posizione.y * MULTIPLY, WHITE);
}
