#pragma once


#include "specs.hpp"

class PowerUp
{
public:
	PowerUp(int type);
	~PowerUp();

	void Draw();
	void Update(float deltaT);

	static void Init();          
    static inline ByteMask byteMask;

	Rectangle getBounds();
	Vector2 position;
	ModifierType modType;
	static inline Sound audio = {};
	static inline float volume = 1.0f;

private:
	int vel = 200;
	Texture2D* texture = nullptr;
	static inline Texture2D arraymodifiers[5] = { 0 };
};