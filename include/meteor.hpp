#pragma once

#include "specs.hpp"

class Meteor
{
public:

	Meteor();
	~Meteor();

	static void Init();
	void Draw();
    void Update(float deltaT);
	
	void NextState();
	MeteorState getState();
	Rectangle getBounds();

	Vector2 position{ 0, 0 };

    static inline ByteMask byteMask;
	static inline float volume = 1.0f;

private:
	int velX = 0;
	int velY = 0;
	float direction = 0;

	Texture2D* texture = nullptr;
	MeteorState AsteroidState = INTEGRITY;
	static inline Sound audio = {};
	static inline Texture2D arraymeteors[2] = { 0 };
};
