#include "explosion.hpp"

Esplosione::Esplosione(const Rectangle& bounds)
{
	SetSoundVolume(audioEsplosione, volume);
	PlaySound(audioEsplosione);

	posizione.x = (bounds.x + bounds.width / 4);
	posizione.y = (bounds.y + bounds.height / 4);
}
void Esplosione::distruggi() 
{ 
	fine = true; 
}
void Esplosione::Aggiorna(float deltaT)
{
	if (index < 21)
	{
		DrawTexture(arrayEsplosione[(int)index], posizione.x, posizione.y, WHITE);
		index += 25 * deltaT;
	}
	else
	{
		distruggi();
	}
}
void Esplosione::Init()
{
	if (loadedResources)
	{
		for (int i = 0; i < 21; i++)
		{
			arrayEsplosione[i] = AssetsManager::GetTexture(TextFormat("frame_explosion_%d", i));
		}

		audioEsplosione = AssetsManager::GetSound("explosion");
	}
}