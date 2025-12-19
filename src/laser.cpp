#include "laser.hpp"

Laser::Laser(int posX, int posY, int laserTimeToLive) : shouldDieTimer(laserTimeToLive, nullptr, false, true)
{  
    SetSoundVolume(audio, volume);
    PlaySound(audio);

	vel = 400;
	posizione.x = static_cast<float>(posX);  
	posizione.y = static_cast<float>(posY);  
}
Laser::~Laser()
{  
} 
Rectangle Laser::getBounds()
{
    return { posizione.x, posizione.y, static_cast<float>(immagine.width), static_cast<float>(immagine.height) };
}

void Laser::Init()
{
    if (loadedResources)
    {
        immagine = AssetsManager::GetTexture("laser");
        byteMask.loadFromImage(PATH_BYTEMASK_LASER);
        audio = AssetsManager::GetSound("laser_shot");
    }
}

void Laser::Disegna()  
{  
    DrawTexture(Laser::immagine, posizione.x, posizione.y, {255, 255, 255, static_cast<unsigned char>(255.0f * alpha)});
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
    return { posizione.x, posizione.y, static_cast<float>(immagine.width), static_cast<float>(immagine.height) };
}

void BigLaser::Init()
{
    if (loadedResources)
    {
        immagine = AssetsManager::GetTexture("big_laser");
        byteMask.loadFromImage(PATH_BYTEMASK_BIGLASER);
    }
}

void BigLaser::Disegna()
{
    DrawTexture(BigLaser::immagine, posizione.x, posizione.y, WHITE);
}