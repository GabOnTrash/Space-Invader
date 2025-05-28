#include "asteroide.hpp"

Asteroide::Asteroide(Vector2 PlayerPosition)
{
	float speed = GetRN<float>(500, 550) * SCALE;

	int fromWhere = GetRN<int>(0, 3);
	// sinistra
	if (fromWhere == 0)
	{
		posizione.x = GetRN<float>(PlayerPosition.x - WINDOW_WIDTH / 2 - 200, PlayerPosition.x - WINDOW_WIDTH / 2 - 100);
		posizione.y = GetRN<float>(PlayerPosition.y - WINDOW_HEIGHT / 2, PlayerPosition.y + WINDOW_HEIGHT / 2);
	}
	// destra
	else if (fromWhere == 1)
	{
		posizione.x = GetRN<float>(PlayerPosition.x + WINDOW_WIDTH / 2 + 100, PlayerPosition.x + WINDOW_WIDTH / 2 + 200);
		posizione.y = GetRN<float>(PlayerPosition.y - WINDOW_HEIGHT / 2, PlayerPosition.y + WINDOW_HEIGHT / 2);
	}
	// sopra
	else if (fromWhere == 2)
	{
		posizione.x = GetRN<float>(PlayerPosition.x - WINDOW_WIDTH / 2, PlayerPosition.x + WINDOW_WIDTH / 2);
		posizione.y = GetRN<float>(PlayerPosition.y + WINDOW_HEIGHT / 2 + 100, PlayerPosition.y + WINDOW_HEIGHT / 2 + 200);
	}
	// sotto
	else if (fromWhere == 3)
	{
		posizione.x = GetRN<float>(PlayerPosition.x - WINDOW_WIDTH / 2, PlayerPosition.x + WINDOW_WIDTH / 2);
		posizione.y = GetRN<float>(PlayerPosition.y - WINDOW_HEIGHT / 2 - 200, PlayerPosition.y - WINDOW_HEIGHT / 2 - 100);
	}

	Vector2 target = 
	{
		PlayerPosition.x + GetRN<float>(-30, 30),
		PlayerPosition.y + GetRN<float>(-30, 30)
	};
	
	Vector2 direction = 
	{
		target.x - posizione.x,
		target.y - posizione.y
	};
	
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) 
	{
		direction.x /= length;
		direction.y /= length;
	}

	// Imposta velocità verso il player
	velX = direction.x * speed * SCALE;
	velY = direction.y * speed * SCALE;

	// Assegna l'immagine
	immagine = &arrayAsteroidi[0];
}
Asteroide::~Asteroide()
{
	// UnloadTexture(immagine);
}
Rectangle Asteroide::getBounds()
{
	return { posizione.x, posizione.y, static_cast<float>(immagine->width * SCALE), static_cast<float>(immagine->height * SCALE) };
}

void Asteroide::Init()
{
	if (!textureCaricata)
	{
		arrayAsteroidi[0] = LoadTexture("immagini/meteor.png");
		arrayAsteroidi[1] = LoadTexture("immagini/meteor2.png");
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
	DrawTextureEx(*immagine, posizione, 0, SCALE, WHITE);
}
void Asteroide::Movimento(float deltaT)
{
	posizione.x += velX * deltaT;
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
