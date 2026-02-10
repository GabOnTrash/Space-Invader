#pragma once

#include "../util/specs.hpp"
#include "../AssetsManager/AssetsManager.hpp"

class Explosion 
{
public:
	Explosion(const Rectangle& bounds);
	void Draw();
	bool ShouldDie(float checkValue = 0);
	void Update(float dt);
	Rectangle getBounds() { return { position.x, position.y, static_cast<float>(textureArray[0].width), static_cast<float>(textureArray[0].height) }; };

	bool end = false;

private:
	void InitResources();
	float index = 0.0f;
    static inline Texture2D textureArray[21] = {0};
	Vector2 position = { 0.0f, 0.0f };

public:
	static inline Sound explosionAudio = {};
	static inline float volume = 1.0f;
};