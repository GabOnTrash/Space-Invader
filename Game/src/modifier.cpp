#include "modifier.hpp"

Modifier::Modifier(int type)
{
    texture = &arraymodifiers[type];
    position.x = GetRN<float>(static_cast<float>(texture->width), static_cast<float>(ViewPort::BASE_WIDTH - texture->width));
    position.y = GetRN<float>(-150.f, -100.f);

    modType = static_cast<ModifierType>(type);
}
Modifier::~Modifier()
{
}
Rectangle Modifier::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture->width), static_cast<float>(texture->height) };
}
void Modifier::Init()
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

void Modifier::Draw()
{
    DrawTexture(*texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}
void Modifier::Update(float deltaT)
{
    position.y += vel * deltaT;
}