#include "Explosion.hpp"

Explosion::Explosion(const Rectangle& bounds)
{
	InitResources();

	SetSoundVolume(explosionAudio, volume);
	PlaySound(explosionAudio);

	position.x = (bounds.x + bounds.width / 2);
	position.y = (bounds.y + bounds.height / 2);
}

void Explosion::Draw()
{
	DrawTexture(textureArray[(int)index], static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}

bool Explosion::ShouldDie(float checkValue)
{
	return end;
}

void Explosion::Update(float dt)
{
    if (index > 21)
    {
        end = true;
        return;
    }
	
	index += 25 * dt;
}

void Explosion::InitResources()
{
	for (int i = 0; i < 21; i++)
	{
		textureArray[i] = AssetsManager::GetTexture(TextFormat("frame_explosion_%d", i));
	}

	explosionAudio = AssetsManager::GetSound("explosion");
}
