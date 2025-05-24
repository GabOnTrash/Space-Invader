#include "astronave.hpp"

Astronave::Astronave()
	: tLaser(coolDown)
{
	Reset();
}
Astronave::~Astronave()
{
	// UnloadTexture(immagine);
}

void Astronave::Init()
{
	if (!textureCaricata)
	{
		immagine = LoadTexture("immagini/giocatore.png");
		textureCaricata = true;
	}
}
void Astronave::Unload()
{
	if (textureCaricata)
	{
		UnloadTexture(immagine);
		textureCaricata = false;
	}
}

void Astronave::Reset()
{
	lasers.clear();
	lasers.reserve(10);

	posizione.x = ((WINDOW_WIDTH / 2.0f) - (immagine.width * SCALE / 2.0f));
	posizione.y = (WINDOW_HEIGHT / 2.0f);
	vel = 250 * SCALE;

	tLaser.deactive();

	tripleLaser = false;
	reducedVel = false;
	continued = false;
	clearLaser();
}

void Astronave::Disegna()
{
	if (continued)
		bigLaser.Disegna();

	for (auto& laser : lasers)
		laser.Disegna();

	DrawTextureEx(immagine, posizione, 0.0f, SCALE, WHITE);
}

void Astronave::Aggiorna(float deltaT)
{
	Movimento(deltaT);
	tLaser.update();

	for (size_t i = 0; i < lasers.size(); )
	{
		lasers[i].Aggiorna(deltaT);

		if (lasers[i].posizione.y < -50 * SCALE)
			lasers.erase(lasers.begin() + i);
		else
			i++;
	}
}

void Astronave::clearLaser()
{
	lasers.clear();
}

void Astronave::Movimento(float deltaT)
{
	if (reducedVel) vel = 150 * SCALE;
	else vel = 250 * SCALE;

	// Input direzione
	if (IsKeyDown(KeyBinds.KeyUP)) direzione.y -= 1;
	if (IsKeyDown(KeyBinds.KeyDOWN)) direzione.y += 1;
	if (IsKeyDown(KeyBinds.KeyLEFT)) direzione.x -= 1;
	if (IsKeyDown(KeyBinds.KeyRIGHT)) direzione.x += 1;

	if (sqrtf(direzione.x * direzione.x + direzione.y * direzione.y) != 0)
	{
		direzione.x /= sqrtf(direzione.x * direzione.x + direzione.y * direzione.y);
		direzione.y /= sqrtf(direzione.x * direzione.x + direzione.y * direzione.y);
	}

	nuovaPosizione.x = posizione.x + direzione.x * vel * deltaT;
	nuovaPosizione.y = posizione.y + direzione.y * vel * deltaT;

	if (nuovaPosizione.x >= 0 && nuovaPosizione.x <= WINDOW_WIDTH - immagine.width * SCALE)
		posizione.x = nuovaPosizione.x;

	if (nuovaPosizione.y >= 0 && nuovaPosizione.y <= WINDOW_HEIGHT - immagine.height * SCALE)
		posizione.y = nuovaPosizione.y;

	if (continued)
	{
		bigLaser.posizione.x = posizione.x + (immagine.width * SCALE / 2.0f) - (BigLaser::immagine.width * SCALE / 2.0f);
		bigLaser.posizione.y = posizione.y - (BigLaser::immagine.height * SCALE);
	}
	else if (IsKeyPressed(KeyBinds.KeySHOOT) && !tLaser.isRunning)
	{
		const float laserOffset = 15.0f * SCALE;
		const float laserCenterX = posizione.x + (immagine.width * SCALE / 2.0f) - (Laser::immagine.width * SCALE / 2.0f);
		const float laserY = posizione.y - laserOffset;

		lasers.emplace_back(laserCenterX, laserY);

		if (tripleLaser)
		{
			lasers.emplace_back(laserCenterX - laserOffset, laserY);
			lasers.emplace_back(laserCenterX + laserOffset, laserY);
		}
		tLaser.active();
	}

	direzione.x = 0;
	direzione.y = 0;
}

Rectangle Astronave::getBounds()
{
	return {
		posizione.x,
		posizione.y,
		immagine.width * SCALE,
		immagine.height * SCALE
	};
}

std::vector<Laser>& Astronave::GetLasers()
{
	return this->lasers;
}
