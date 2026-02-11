#pragma once

#include "../util/specs.hpp"
#include "../AssetsManager/AssetsManager.hpp"

class Modifier
{
public:
	Modifier(Vector2 position, int type);
	~Modifier();
	void InitAllTextures();
	void Draw();
	void Update(float dt);
	bool ShouldDie(float checkValue);
	Rectangle getBounds();

	Vector2 position;
	ModifierType modType;
	static inline Sound audio = {};
    static inline ByteMask byteMask;
	static inline float volume = 1.0f;

private:
	int vel = 300;
	Texture2D* texture = nullptr;
	static inline Texture2D arraymodifiers[5] = { 0 };
};