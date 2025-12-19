#pragma once

#include "specs.hpp"

class Esplosione 
{
public:

	Esplosione(const Rectangle& bounds);
	void distruggi();
	void Aggiorna(float deltaT);
	static void Init();            

	float index = 0.0f;
	bool fine = false;

	Vector2 posizione = { 0.0f, 0.0f };

	static inline Texture2D arrayEsplosione[21] = { 0 };
	static inline Sound audioEsplosione = {};
	static inline float volume = 1.0f;
};