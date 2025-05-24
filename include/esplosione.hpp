#pragma once

#include "specs.hpp"

class Esplosione 
{
public:

	Esplosione(const Rectangle& bounds);
	void distruggi();
	void Aggiorna(float deltaT);
	static void Init();            // <--- chiama questo dopo InitWindow()
	static void Unload();          // <--- da chiamare prima di CloseWindow()

	float index = 0.0f;
	bool fine = false;

	Vector2 posizione = { 0.0f, 0.0f };

	static inline Texture2D arrayEsplosione[21] = { 0 };
	static inline Sound audioEsplosione = {};
	static inline bool textureCaricata = false;   // flag per sapere se è già stata caricata
	static inline float volume = 1.0f;
};