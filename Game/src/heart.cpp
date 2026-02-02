#include "heart.hpp"

Rectangle Heart::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}
void Heart::Init()
{
    if (ViewPort::loadedResources) 
        texture = AssetsManager::GetTexture("heart");
}

void Heart::Draw()
{
    DrawTexture(Heart::texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}
