#include "laser.hpp"

Laser::Laser(int posX, int posY) 
{  
    SetSoundVolume(audio, volume);
    PlaySound(audio);

	vel = 400 * SCALE;
	posizione.x = static_cast<float>(posX);  
	posizione.y = static_cast<float>(posY);  
}  
Laser::~Laser()  
{  
} 
Rectangle Laser::getBounds()
{
    return { posizione.x, posizione.y, static_cast<float>(immagine.width) * SCALE, static_cast<float>(immagine.height) * SCALE };
}

void Laser::Init()
{
    if (!textureCaricata)
    {
        immagine = LoadTexture("immagini/laser.png");
        audio = LoadSound("immagini/laser.wav");
        textureCaricata = true;
    }
}
void Laser::Unload()
{
    if (textureCaricata)
    {
        UnloadTexture(immagine);
        UnloadSound(audio);
        textureCaricata = false;
    }
}
void Laser::Disegna()  
{  
    DrawTextureEx(Laser::immagine, posizione, 0, SCALE, WHITE);
}  
void Laser::Movimento(float deltaT)  
{  
	posizione.y -= vel * deltaT;
}  
void Laser::Aggiorna(float deltaT)  
{  
    Movimento(deltaT);  
}


BigLaser::BigLaser()
{
}
BigLaser::~BigLaser()
{
}
Rectangle BigLaser::getBounds()
{
    return { posizione.x, posizione.y, static_cast<float>(immagine.width) * SCALE, static_cast<float>(immagine.height) * SCALE };
}

void BigLaser::Init()
{
    if (!textureCaricata)
    {
        immagine = LoadTexture("immagini/bigLaser.png");
        textureCaricata = true;
    }
}
void BigLaser::Unload()
{
    if (textureCaricata)
    {
        UnloadTexture(immagine);
        textureCaricata = false;
    }
}
void BigLaser::Disegna()
{
    DrawTextureEx(BigLaser::immagine, posizione, 0, SCALE, WHITE);
}