#pragma once

#include "specs.hpp"

class Cuore
{
public:

	Cuore() {}

	static void Init();
	static void Unload();          
	void Disegna();
	inline int getWidth() { return Cuore::immagine.width * SCALE; }

	Rectangle getBounds();
	Vector2 posizione = { 0, 0 };

private:

	static inline Texture2D immagine = { 0 };
	static inline bool textureCaricata = false;
};