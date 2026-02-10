#include "Laser.hpp"

Laser::Laser(Vector2 position, int laserTimeToLive)
    : shouldDieTimer(laserTimeToLive, nullptr, false, true)
{
    texture = AssetsManager::GetTexture("laser");
    audio = AssetsManager::GetSound("laser_shot");

    static bool byteMaskLoaded = false;
    if (!byteMaskLoaded)
    {
        byteMask.loadFromImage(PATH_BYTEMASK_LASER);
        byteMaskLoaded = true;
    }
    SetSoundVolume(audio, volume);
    PlaySound(audio);

	this->position = position;
}
Laser::~Laser()
{  
} 
Rectangle Laser::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}

void Laser::Draw()  
{  
    DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), {255, 255, 255, static_cast<unsigned char>(255.0f * alpha)});
}  
void Laser::Update(float dt)
{  
    position.y -= vel * dt;
    shouldDieTimer.update();
    alpha = (1.0f - static_cast<float>(shouldDieTimer.elapsedTime()) / static_cast<float>(shouldDieTimer.duration.count())); // alpha updates only if the game is being played
}
bool Laser::ShouldDie(float checkValue)
{
    return !shouldDieTimer.isRunning || (position.y < -texture.height);
}

BigLaser::BigLaser()
{
    texture = AssetsManager::GetTexture("big_laser");
    byteMask.loadFromImage(PATH_BYTEMASK_BIGLASER);
}
BigLaser::~BigLaser()
{
}
Rectangle BigLaser::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}
void BigLaser::Draw()
{
    DrawTexture(BigLaser::texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}