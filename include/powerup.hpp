#pragma once


#include "specs.hpp"

class PowerUp
{
public:

	PowerUp(int tipo);
	~PowerUp();

	void Disegna();
	void Movimento(float deltaT);
	void Aggiorna(float deltaT);

	static void Init();          
    static inline ByteMask byteMask;

	Rectangle getBounds();

	Texture2D* immagine = nullptr;
	Vector2 posizione;
	int vel = 200;

	std::string tipoPotenziamento = "";

	static inline Texture2D arrayPotenziamenti[5] = { 0 };
	static inline Sound audio = {};
	static inline float volume = 1.0f;
};