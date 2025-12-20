#include "laser.hpp"

Laser::Laser(int posX, int posY, int laserTimeToLive) : shouldDieTimer(laserTimeToLive, nullptr, false, true)
{  
    SetSoundVolume(audio, volume);
    PlaySound(audio);

	position.x = static_cast<float>(posX);  
	position.y = static_cast<float>(posY);  
}
Laser::~Laser()
{  
} 
Rectangle Laser::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}

void Laser::Init()
{
    if (loadedResources)
    {
        texture = AssetsManager::GetTexture("laser");
        byteMask.loadFromImage(PATH_BYTEMASK_LASER);
        audio = AssetsManager::GetSound("laser_shot");
    }
}

void Laser::Draw()  
{  
    DrawTexture(Laser::texture, position.x, position.y, {255, 255, 255, static_cast<unsigned char>(255.0f * alpha)});
}  
void Laser::Update(float deltaT)  
{  
    position.y -= vel * deltaT;
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
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}

void BigLaser::Init()
{
    if (loadedResources)
    {
        texture = AssetsManager::GetTexture("big_laser");
        byteMask.loadFromImage(PATH_BYTEMASK_BIGLASER);
    }
}

void BigLaser::Draw()
{
    DrawTexture(BigLaser::texture, position.x, position.y, WHITE);
}