#include "player.hpp"

Astronave::Astronave()
	: tLaser(coolDown)
{
	Reset();
}
Astronave::~Astronave()
{
}

void Astronave::Init()
{
	if (loadedResources)
	{
		immagine = AssetsManager::GetTexture("player");
        byteMask.loadFromImage(PATH_BYTEMASK_PLAYER);
	}
}

void Astronave::Reset()
{
	lasers.clear();
	lasers.reserve(10);

	posizione.x = ((BASE_WIDTH / 2.0f) - (immagine.width / 2.0f));
	posizione.y = (BASE_HEIGHT / 2.0f);
	vel = 250;

	tLaser.deactive();

	tripleLaser = false;
	reducedVel = false;
	continued = false;
	clearLaser();
}

void Astronave::Disegna()
{
    DrawTexture(immagine, posizione.x, posizione.y, WHITE);
}

void Astronave::Aggiorna(float deltaT)
{
	Movimento(deltaT);
	tLaser.update();
}

void Astronave::clearLaser()
{
	lasers.clear();
}

void Astronave::Movimento(float deltaT)
{
	if (reducedVel) vel = 150;
	else vel = 250;

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

	if (nuovaPosizione.x >= 0 && nuovaPosizione.x <= BASE_WIDTH - immagine.width)
		posizione.x = nuovaPosizione.x;

	if (nuovaPosizione.y >= 0 && nuovaPosizione.y <= BASE_HEIGHT - immagine.height)
		posizione.y = nuovaPosizione.y;

	if (continued)
	{
		bigLaser.posizione.x = posizione.x + (immagine.width / 2.0f) - (BigLaser::immagine.width / 2.0f);
		bigLaser.posizione.y = posizione.y - (BigLaser::immagine.height);
	}
	else if (IsKeyPressed(KeyBinds.KeySHOOT) && !tLaser.isRunning)
	{
		const float laserOffset = 30.0f * scale;
		const float laserCenterX = posizione.x + (immagine.width / 2.0f) - (Laser::immagine.width / 2.0f);
		const float laserY = posizione.y - laserOffset;

		lasers.emplace_back(laserCenterX, laserY, laserTimeToLive);

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
    return { posizione.x, posizione.y, static_cast<float>(immagine.width), static_cast<float>(immagine.height) };
}

std::vector<Laser>& Astronave::GetLasers()
{
	return this->lasers;
}
