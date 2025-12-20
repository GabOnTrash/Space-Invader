#pragma once

#include "specs.hpp"
#include "timer.hpp"

class Laser
{
public: 

	Laser(int posX, int posY, int laserTimeToLive = 2000);
	~Laser();

	void Draw();
    void Update(float deltaT);

    bool shouldDie();
	static void Init(); 

	Rectangle getBounds();

	Vector2 position = { 0, 0 };

	static inline ByteMask byteMask;
	static inline float volume = 1.0f;
	static inline Texture2D texture = { 0 };
	
private:
	int vel = 400;
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
	static void Init();

	Rectangle getBounds();
	Vector2 position = { 0, 0 };

	static inline ByteMask byteMask;
	static inline Texture2D texture = { 0 };
};