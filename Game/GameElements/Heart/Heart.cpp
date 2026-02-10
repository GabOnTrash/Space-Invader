#include "Heart.hpp"

Heart::Heart()
{
    texture = AssetsManager::GetTexture("heart");
}

Rectangle Heart::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}

void Heart::Draw()
{
    DrawTexture(Heart::texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}
