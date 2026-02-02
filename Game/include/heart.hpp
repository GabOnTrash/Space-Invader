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
	static inline Texture2D texture = { 0 };
};
