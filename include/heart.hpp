#pragma once

#include "specs.hpp"

class Heart
{
public:
    Heart() = default;

	static void Init(); 
	void Draw();

	Rectangle getBounds();
	Vector2 position = { 0, 0 };

private:
	const float MULTIPLY = 1.5f * scale;
	static inline Texture2D texture = { 0 };
};