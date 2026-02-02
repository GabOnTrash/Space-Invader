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
	static inline Vector2 playerPos = { 0.0f, 0.0f };
private:
	int velX = 0;
	int velY = 0;
	Vector2 direction = {0.0f, 0.0f};

	Texture2D* texture = nullptr;
	MeteorState AsteroidState = INTEGRITY;
	static inline Sound audio = {};
	static inline Texture2D arraymeteors[2] = { 0 };
};
