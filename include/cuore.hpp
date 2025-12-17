#pragma once

#include "specs.hpp"

class Cuore
{
public:
	Cuore() {}

	static void Init();
	static void Unload();          
	void Disegna();
	inline int getWidth() { return Cuore::immagine.width * MULTIPLY; }

	Rectangle getBounds();
	Vector2 posizione = { 0, 0 };

private:
	const float MULTIPLY = 1.5f;

	static inline Texture2D immagine = { 0 };
	static inline bool textureCaricata = false;
};