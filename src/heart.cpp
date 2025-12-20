#include "heart.hpp"

Rectangle Heart::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}
void Heart::Init()
{
    if (loadedResources) 
        texture = AssetsManager::GetTexture("heart");
}

void Heart::Draw()
{
    DrawTextureEx(Heart::texture, position, 0.0f, MULTIPLY, WHITE);
}
