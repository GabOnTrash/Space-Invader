#include "heart.hpp"

Rectangle Cuore::getBounds()
{
    return { posizione.x, posizione.y, static_cast<float>(immagine.width), static_cast<float>(immagine.height) };
}
void Cuore::Init()
{
    if (loadedResources) immagine = AssetsManager::GetTexture("heart");
}

void Cuore::Disegna()
{
    DrawTexture(Cuore::immagine, posizione.x, posizione.y * MULTIPLY, WHITE);
}
