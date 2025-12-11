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

	void Disegna();
	void Movimento(float deltaT);
    void Aggiorna(/*Vector2& player, */float deltaT);
	static void Init();
	static void Unload();
	void NextState();
	MeteorState getState();
	MeteorState AsteroidState = INTEGRITY;
	Rectangle getBounds();

	Vector2 posizione{ 0, 0 };
	Texture2D* immagine = nullptr;

	static inline Texture2D arrayAsteroidi[2] = { 0 };
	static inline Sound audio = {};
	static inline float volume = 1.0f;
	static inline bool textureCaricata = false;

/*private:
	Vector2 direzione{0, 1};
    float velocita = 200.0f;
    void SeguiGiocatore(Vector2 playerPos, float deltaT);

    float tempoDaUltimoRicalcolo = 0.0f;
    float intervalloRicalcolo = 1.5f; */ 
};
