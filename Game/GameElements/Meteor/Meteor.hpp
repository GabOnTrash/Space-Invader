#pragma once

#include "../util/specs.hpp"
#include "../AssetsManager/AssetsManager.hpp"

class Meteor
{
public:
	Meteor(Vector2 pos);
	~Meteor();

	void Draw();
    void Update(float dt);
	bool ShouldDie(float checkValue = 0);
	void NextState();
	Rectangle getBounds();

	Vector2 position{ 0, 0 };

    static inline ByteMask byteMask;
	static inline float volume = 1.0f;
	static inline Vector2 playerPos = { 0.0f, 0.0f };

	bool shouldDie = false;

private:
	Vector2 velocity{ 0.0f, 0.0f };
	Vector2 direction{0.0f, 0.0f};

	Texture2D* texture = nullptr;
	MeteorState meteorState = MeteorState::INTEGRITY;
	static inline Texture2D arraymeteors[2]{};
	static inline Sound damage{};
};
