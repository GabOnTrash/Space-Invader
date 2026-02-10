#include "Modifier.hpp"

Modifier::Modifier(Vector2 position, int type)
{
    InitAllTextures();

    texture = &arraymodifiers[type];
    this->position = position;
    modType = static_cast<ModifierType>(type);
}
Modifier::~Modifier()
{
}
Rectangle Modifier::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture->width), static_cast<float>(texture->height) };
}
void Modifier::InitAllTextures()
{
    arraymodifiers[0] = AssetsManager::GetTexture("reduced_vel");
    arraymodifiers[1] = AssetsManager::GetTexture("minus_one_heart");
    arraymodifiers[2] = AssetsManager::GetTexture("triple_laser");
    arraymodifiers[3] = AssetsManager::GetTexture("plus_one_heart");
    arraymodifiers[4] = AssetsManager::GetTexture("_big_laser");

    static bool byteMaskLoaded = false;
    if (!byteMaskLoaded)
    {
        byteMask.loadFromImage(PATH_BYTEMASK_MODIFIERS);
        byteMaskLoaded = true;
    }
    audio = AssetsManager::GetSound("modifier_sound");
}

void Modifier::Draw()
{
    DrawTexture(*texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}
void Modifier::Update(float dt)
{
    position.y += vel * dt;
}

bool Modifier::ShouldDie(float checkValue)
{
    return position.y > checkValue + texture->height;
}
