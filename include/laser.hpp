#pragma once

#include "specs.hpp"

class Laser
{
public: 

	Laser(int posX, int posY);
	~Laser();

	void Disegna();
	void Movimento(float deltaT);
	void Aggiorna(float deltaT);

	static void Init();            // <--- chiama questo dopo InitWindow()
	static void Unload();          // <--- da chiamare prima di CloseWindow()

	Rectangle getBounds();

	Vector2 posizione = { 0, 0 };
	int vel = 0;

	static inline Texture2D immagine = { 0 };
	static inline Sound audio = {};
	static inline float volume = 1.0f;
	static inline bool textureCaricata = false;
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

	static inline Texture2D immagine = { 0 };
	static inline bool textureCaricata = false;
};