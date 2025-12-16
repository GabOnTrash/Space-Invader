#pragma once

#include "specs.hpp"
#include "timer.hpp"

class Laser
{
public: 

	Laser(int posX, int posY, int laserTimeToLive = 2000);
	~Laser();

	void Disegna();
	void Movimento(float deltaT);
    void Aggiorna(float deltaT);

    bool shouldDie();

	static void Init();            // <--- chiama questo dopo InitWindow()
	static void Unload();          // <--- da chiamare prima di CloseWindow()

	Rectangle getBounds();

	Vector2 posizione = { 0, 0 };
	int vel = 0;
    float alpha = 0;

	static inline ByteMask byteMask;
	static inline Texture2D immagine = { 0 };
	static inline float volume = 1.0f;
	

private:
	static inline Sound audio = {};
	static inline bool textureCaricata = false;

	Timer<> shouldDieTimer;
};

class BigLaser
{
public:

	BigLaser();
	~BigLaser();

	void Disegna();

	static void Init();
	static void Unload();

	Rectangle getBounds();
	Vector2 posizione = { 0, 0 };

	static inline ByteMask byteMask;
	static inline Texture2D immagine = { 0 };
	static inline bool textureCaricata = false;
};