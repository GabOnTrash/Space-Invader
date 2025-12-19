#pragma once

#include "specs.hpp"

class Asteroide
{
public:

	Asteroide();
	~Asteroide();

	int velX = 0;
	int velY = 0;
	float verso = 0;

	static void Init();
	
	void Disegna();
	void Movimento(float deltaT);
    void Aggiorna(float deltaT);
	void NextState();
	MeteorState getState();
	Rectangle getBounds();

	Vector2 posizione{ 0, 0 };
	Texture2D* immagine = nullptr;

    static inline ByteMask byteMask;
	static inline Texture2D arrayAsteroidi[2] = { 0 };
	static inline Sound audio = {};
	static inline float volume = 1.0f;

private:
	MeteorState AsteroidState = INTEGRITY;
};
