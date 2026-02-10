#pragma once

#include "raylib.h"
#include "../GameElements/AssetsManager/AssetsManager.hpp"

class Heart
{
public:
    Heart();

	void Draw();
	Rectangle getBounds();

	Vector2 position = { 0, 0 };

private:
	static inline Texture2D texture = {};
};
