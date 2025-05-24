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

	static void Init();            // <--- chiama questo dopo InitWindow()
	static void Unload();          // <--- da chiamare prima di CloseWindow()

	Rectangle getBounds();

	Texture2D* immagine = nullptr;
	Vector2 posizione;
	int vel = 0;

	std::string tipoPotenziamento = "";

	static inline Texture2D arrayPotenziamenti[5] = { 0 };
	static inline Sound audio = {};
	static inline float volume = 1.0f;
	static inline bool textureCaricata = false;   // flag per sapere se è già stata caricata
};