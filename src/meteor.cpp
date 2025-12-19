#include "meteor.hpp"

Asteroide::Asteroide()
{
    immagine = arrayAsteroidi;

	velX = GetRN<int>(250, 300);
	velY = GetRN<int>(250, 300);
	verso =	GetRN<float>(-1, 1);
	posizione.x = GetRN<float>(50, BASE_WIDTH - 50);
    posizione.y = GetRN<float>(-immagine->height * 2.0f, -immagine->height * 1.5f);

	if (verso == 0)
		verso = 1;

	verso *= 0.7f;
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
	if (loadedResources)
	{
		arrayAsteroidi[0] = AssetsManager::GetTexture("meteor");
        arrayAsteroidi[1] = AssetsManager::GetTexture("meteor_damaged");
        byteMask.loadFromImage(PATH_BYTEMASK_ASTEROID);

		audio = AssetsManager::GetSound("meteor_damage");
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
