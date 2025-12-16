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
    static inline ByteMask byteMask;

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
	void setLaserTimeToLive(int timeInMs)
	{
		laserTimeToLive = timeInMs;
    }

	std::vector<Laser>& GetLasers();

	Rectangle getBounds();
	BigLaser bigLaser;
	Vector2 posizione{ 0, 0 };
	std::vector<Laser> lasers;

private:

	int laserTimeToLive = 2000; // in ms

	Vector2 direzione{ 0, 0 };
	Vector2 nuovaPosizione{ 0, 0 };

	Timer<> tLaser;

	static inline Texture2D immagine = { 0 };
	static inline bool textureCaricata = false;
};
