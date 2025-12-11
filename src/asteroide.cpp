#include "asteroide.hpp"

Asteroide::Asteroide()
{
	velX = GetRN<int>(250, 300) * SCALE;
	velY = GetRN<int>(250, 300) * SCALE;
	verso =	GetRN<float>(-1, 1);
	posizione.x = GetRN<float>(0, WINDOW_WIDTH);
	posizione.y = GetRN<float>(-150, -100);

	if (verso == 0)
		verso = 1;

	verso *= 0.7f;

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
/* void Asteroide::SeguiGiocatore(Vector2 playerPos, float deltaT)
{
    tempoDaUltimoRicalcolo += deltaT;

	if (tempoDaUltimoRicalcolo >= intervalloRicalcolo && posizione.y < playerPos.y && abs(posizione.x - posizione.x) < 200)
    {
        tempoDaUltimoRicalcolo = 0.0f;

        float dx = playerPos.x - posizione.x;
        float dy = playerPos.y - posizione.y;
        float distanza = sqrtf(dx * dx + dy * dy);

		if (abs(direzione.x - verso) > .3f)
            return;

        if (distanza > 1.0f)
        {
            dx /= distanza;
            dy /= distanza;
        }

        // Fattore di influenza della velocità:
        // se la velocità è alta, corregge meno
        float fattoreCurvatura = 1.0f / (1.0f + velocita / 250.0f);

        // Interpolazione verso la nuova direzione (curvatura naturale)
        direzione.x = direzione.x * (1.0f - fattoreCurvatura) + dx * fattoreCurvatura;
        direzione.y = direzione.y * (1.0f - fattoreCurvatura) + dy * fattoreCurvatura;

        // Normalizza per evitare che la direzione “cresca” nel tempo
        float len = sqrtf(direzione.x * direzione.x + direzione.y * direzione.y);
        if (len > 0.001f)
        {
            direzione.x /= len;
            direzione.y /= len;
        }
    }

    // Movimento continuo nella direzione attuale
    posizione.x += direzione.x * velocita * deltaT;
    posizione.y += direzione.y * velocita * deltaT;
}*/
void Asteroide::Movimento(float deltaT)
{
	posizione.x += velX * verso * deltaT;
	posizione.y += velY * deltaT;
}
void Asteroide::Aggiorna(/*Vector2& player, */float deltaT)
{
    // SeguiGiocatore(player, deltaT);
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
