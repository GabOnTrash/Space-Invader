#pragma once

#include "specs.hpp"

class Explosion 
{
public:
	Explosion(const Rectangle& bounds);
	void Update(float deltaT);
	static void Init();            

	bool end = false;

private:
	float index = 0.0f;
    static inline Texture2D textureArray[21] = {0};
	Vector2 position = { 0.0f, 0.0f };

public:
	static inline Sound explosionAudio = {};
	static inline float volume = 1.0f;
};