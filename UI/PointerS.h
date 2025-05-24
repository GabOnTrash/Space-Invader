#pragma once

#include "Widget.h"
#include <raylib.h>

class PointerS 
{
private:

	bool rect = true;

	float posX = 0;
	float posY = 0;

	float centerX = 0;
	float centerY = 0;

	float side1 = 0;
	float side2 = 0;
	float roundness = 0;
	float segments = 0;

	Color color = { 0, 0, 0, 0 };

public:

	PointerS() {}
	PointerS(bool rect, float side1, float side2, float roundness, float segments, Color color)
		: rect(rect), side1(side1), side2(side2), roundness(roundness), segments(segments), color(color) 
	{
	}

	// Getters
	bool isRect() const { return rect; }

	float getPosX() const { return posX; }
	float getPosY() const { return posY; }

	float getCenterX() const { return centerX; }
	float getCenterY() const { return centerY; }

	float getSide1() const { return side1; }
	float getSide2() const { return side2; }

	float getRoundness() const { return roundness; }
	float getSegments() const { return segments; }

	Color getColor() const { return color; }

	// Setters
	void setRect(bool value) { rect = value; }

	void setPosX(float value) { posX = value; }
	void setPosY(float value) { posY = value; }

	void setCenterX(float value) { centerX = value; }
	void setCenterY(float value) { centerY = value; }

	void setSide1(float value) { side1 = value; }
	void setSide2(float value) { side2 = value; }

	void setRoundness(float value) { roundness = value; }
	void setSegments(float value) { segments = value; }

	void setColor(Color newColor) { color = newColor; }
};