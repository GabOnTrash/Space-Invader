#include "laser.hpp"

Laser::Laser(int posX, int posY, int laserTimeToLive) : shouldDieTimer(laserTimeToLive, nullptr, false, true)
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
        byteMask.loadFromImage("immagini/laser.png");

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
    DrawTextureEx(Laser::immagine, posizione, 0, SCALE, {255, 255, 255, static_cast<unsigned char>(255.0f * alpha)});
}  
void Laser::Movimento(float deltaT)  
{  
	posizione.y -= vel * deltaT;
}  
void Laser::Aggiorna(float deltaT)  
{  
    Movimento(deltaT);
    shouldDieTimer.update();
    alpha = (1.0f - static_cast<float>(shouldDieTimer.elapsedTime()) / static_cast<float>(shouldDieTimer.duration.count())); // alpha updates only if the game is being played
}
bool Laser::shouldDie()
{
    return !shouldDieTimer.isRunning;
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
        byteMask.loadFromImage("immagini/bigLaser.png");

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