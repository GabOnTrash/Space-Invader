#include "powerup.hpp"

PowerUp::PowerUp(int type)
{
    position.x = GetRN<float>(0, ViewPort::BASE_WIDTH);
    position.y = GetRN<float>(-150, -100);
    
    modType = static_cast<ModifierType>(type);
    texture = &arraymodifiers[type];
}
PowerUp::~PowerUp()
{
}
Rectangle PowerUp::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture->width), static_cast<float>(texture->height) };
}
void PowerUp::Init()
{
    if (ViewPort::loadedResources)
    {
        arraymodifiers[0] = AssetsManager::GetTexture("reduced_vel");
        arraymodifiers[1] = AssetsManager::GetTexture("minus_one_heart");
        arraymodifiers[2] = AssetsManager::GetTexture("triple_laser");
        arraymodifiers[3] = AssetsManager::GetTexture("plus_one_heart");
        arraymodifiers[4] = AssetsManager::GetTexture("_big_laser");

        byteMask.loadFromImage(PATH_BYTEMASK_MODIFIERS);

        audio = AssetsManager::GetSound("modifier_sound");
    }
}

void PowerUp::Draw()
{
    DrawTexture(*texture, position.x, position.y, WHITE);
}
void PowerUp::Update(float deltaT)
{
    position.y += vel * deltaT;
}