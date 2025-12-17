#include "esplosione.hpp"

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
	if (!textureCaricata)
	{
		for (int i = 0; i < 21; i++)
		{
			arrayEsplosione[i] = LoadTexture(TextFormat("immagini/esplosioni/%d.png", i));
		}

		audioEsplosione = LoadSound("immagini/explosion.wav");
		textureCaricata = true;
	}
}
void Esplosione::Unload()
{
	if (textureCaricata)
	{
		for (int i = 0; i < 21; i++)
		{
			UnloadTexture(arrayEsplosione[i]);
		}
		textureCaricata = false;
	}
}