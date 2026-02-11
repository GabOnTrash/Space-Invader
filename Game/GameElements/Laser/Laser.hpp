#pragma once

#include "../util/specs.hpp"
#include "../AssetsManager/AssetsManager.hpp"

class Laser
{
public:
	Laser(Vector2 position, int laserTimeToLive = 2000);
	~Laser();

	void Draw();
    void Update(float dt);
    bool ShouldDie(float checkValue = 0);
	Rectangle getBounds();

	Vector2 position = { 0, 0 };

	static inline ByteMask byteMask;
	static inline float volume = 1.0f;
	static inline Texture2D texture = { 0 };
	
	uint32_t owner;
private:
	int vel = 600;
    float alpha = 0;
	static inline Sound audio = {};
	Timer<> shouldDieTimer;
};

class BigLaser
{
public:
	BigLaser();
	~BigLaser();
	void Draw();
	Rectangle getBounds();

	Vector2 position = { 0, 0 };

	static inline ByteMask byteMask;
	static inline Texture2D texture{};
};