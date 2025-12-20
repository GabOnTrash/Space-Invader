#include "explosion.hpp"

Explosion::Explosion(const Rectangle& bounds)
{
	SetSoundVolume(explosionAudio, volume);
	PlaySound(explosionAudio);

	position.x = (bounds.x + bounds.width / 4);
	position.y = (bounds.y + bounds.height / 4);
}
void Explosion::Update(float deltaT)
{
    if (index > 21)
    {
        end = true;
        return;
    }
	
	DrawTexture(textureArray[(int)index], position.x, position.y, WHITE);
	index += 25 * deltaT;
}
void Explosion::Init()
{
	if (loadedResources)
	{
		for (int i = 0; i < 21; i++)
		{
			textureArray[i] = AssetsManager::GetTexture(TextFormat("frame_explosion_%d", i));
		}

		explosionAudio = AssetsManager::GetSound("explosion");
	}
}