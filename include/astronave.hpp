#pragma once

#include "laser.hpp"
#include "specs.hpp"

class Astronave
{
public:

	Astronave();
	~Astronave();

	static void Init();
	static void Unload();

	void Reset();

	int vel = 0;
	int coolDown = 400;
	bool tripleLaser = false;
	bool reducedVel = false;
	bool continued = false;

	void Aggiorna(float deltaT);
	void Disegna();
	void clearLaser();

	void Movimento(float deltaT);

	Vector2 GetPosition() 
	{ 
		return posizione; 
	}
	Texture2D& GetImage() 
	{ 
		return Astronave::immagine; 
	}
	Timer<>& GetTimer() 
	{ 
		return tLaser; 
	}

	std::vector<Laser>& GetLasers();

	Rectangle getBounds();
	BigLaser bigLaser;

private:

	Vector2 posizione{ 0, 0 };
	Vector2 direzione{ 0, 0 };
	Vector2 nuovaPosizione{ 0, 0 };

	std::vector<Laser> lasers;
	Timer<> tLaser;

	static inline Texture2D immagine = { 0 };
	static inline bool textureCaricata = false;
};
