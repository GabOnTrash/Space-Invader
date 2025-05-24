#pragma once

#include "Widget.h"
#include <cmath>

class Slider : public Widget
{
public:

	Slider() {}
	Slider(std::string id, RectS& rectangle, PointerS& point);

	void draw() override;
	void update() override;
	bool isHovered() override;

	// Getters
	std::string getId() const override { return id; }
	bool getActive() const override { return activated; }

	// Setters
	void setId(std::string newId) override { id = newId; }
	void setActive(bool val) override { activated = val; }

	RectS& getRect() override { return rect; }
	PointerS& getPointer() override { return pointer; }

private:
	
	bool Pressed();
	float GetValue();
	bool hoveredP();
	bool hoveredR();
	void setPointerHoriz();
	void setPointerVert();

	std::string id = "";
	bool activated = true;
	bool vert = true;
	bool clicked = false;
	bool ButtonDown = false;
	float normalized = 0;

	float minY = 0, maxY = 0, rangeY = 0, minX = 0, maxX = 0, rangeX = 0;

	RectS rect;
	PointerS pointer;
	Vector2 mousePos{ 0, 0 };
};