#include "asteroide.hpp"

Asteroide::Asteroide()
{
	velX = GetRN<int>(250, 300);
	velY = GetRN<int>(250, 300);
	verso =	GetRN<float>(-1, 1);
	posizione.x = GetRN<float>(0, BASE_WIDTH);
	posizione.y = GetRN<float>(-150, -100);

	if (verso == 0)
		verso = 1;

	verso *= 0.7f;

	immagine = &arrayAsteroidi[0];
}
Asteroide::~Asteroide()
{
}
Rectangle Asteroide::getBounds()
{
	return { posizione.x, posizione.y, static_cast<float>(immagine->width), static_cast<float>(immagine->height) };
}

void Asteroide::Init()
{
	if (!textureCaricata)
	{
		arrayAsteroidi[0] = LoadTexture("immagini/meteor.png");
		arrayAsteroidi[1] = LoadTexture("immagini/meteor2.png");
        byteMask.loadFromImage("immagini/meteor.png");

		audio = LoadSound("immagini/damage.ogg");
		textureCaricata = true;
	}
}
void Asteroide::Unload()
{
	if (textureCaricata)
	{
		for (int i = 0; i < 2; i++)
			UnloadTexture(arrayAsteroidi[i]);

		UnloadSound(audio);
		textureCaricata = false;
	}
}
void Asteroide::Disegna()
{
	DrawTexture(*immagine, posizione.x, posizione.y, WHITE);
}
void Asteroide::Movimento(float deltaT)
{
	posizione.x += velX * verso * deltaT;
	posizione.y += velY * deltaT;
}
void Asteroide::Aggiorna(float deltaT)
{
	Movimento(deltaT);
}

void Asteroide::NextState()
{
	SetSoundVolume(audio, volume);
	PlaySound(audio);
	AsteroidState = static_cast<MeteorState>(AsteroidState + 1);

	if (AsteroidState == DAMAGED) immagine = &arrayAsteroidi[1];
}
MeteorState Asteroide::getState()
{
	return static_cast<MeteorState>(AsteroidState);
}
