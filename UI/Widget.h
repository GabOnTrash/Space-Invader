#pragma once

#include "PointerS.h"
#include "RectS.h"
#include <raylib.h>
#include <string>

class Widget
{
public:

	virtual void draw() {}
	virtual void update() {}
	virtual bool OnClick() { return false; }
	virtual bool isHovered() { return false; }

	// Getters
	virtual std::string getId() const { return id; }
	virtual std::string getText() const { return text; }
	virtual bool getActive() const { return activated; }

	virtual float getPosX() const { return posX; }
	virtual float getPosY() const { return posY; }
	virtual float getWidth() const { return width; }
	virtual float getHeight() const { return height; }

	virtual float getRoundness() const { return roundness; }
	virtual float getSegments() const { return segments; }
	virtual float getThickness() const { return thickness; }
	
	virtual float getSide1() const { return side1; }
	virtual float getSide2() const { return side2; }

	virtual Color getBG() const { return BG; }
	virtual Color getFG() const { return FG; }
	virtual Color getBD() const { return BD; }
	virtual Color getBDH() const { return BDH; }
	virtual Color getBGH() const { return BGH; }

	virtual RectS& getRect() { return Rect; }
	virtual PointerS& getPointer() { return pointer; }

	// Setters
	virtual void setId(std::string newId) { id = newId; }
	virtual void setText(std::string newText) { text = newText; }
	virtual void setActive(bool val) { activated = val; }

	virtual void setPosX(float value) { posX = value; }
	virtual void setPosY(float value) { posY = value; }
	virtual void setWidth(float value) { width = value; }
	virtual void setHeight(float value) { height = value; }

	virtual void setSide1(float value) { side1 = value; }
	virtual void setSide2(float value) { side2 = value; }

	virtual void setRoundness(float value) { roundness = value; }
	virtual void setSegments(float value) { segments = value; }
	virtual void setThickness(float value) { thickness = value; }

	virtual void setBG(Color value) { BG = value; }
	virtual void setFG(Color value) { FG = value; }
	virtual void setBD(Color value) { BD = value; }
	virtual void setBDH(Color value) { BDH = value; }
	virtual void setBGH(Color value) { BGH = value; }

protected:

	float posX = 0;
	float posY = 0;
	float width = 0;
	float height = 0;

	float centerX = 0;
	float centerY = 0;

	float roundness = 0;
	float segments = 0;
	float thickness = 0;

	float side1 = 0;
	float side2 = 0;

	bool rect = 0;
	float side = 0;
	float min = 0;
	float max = 0;
	float* var = 0;

	Color FG = WHITE;	// TEXT/FG
	Color FGH = WHITE;	// TEXT/FG
	Color BD = WHITE;	// BORDER COLOR
	Color BDH = WHITE;	// BD hover
	Color BG = WHITE;	// BACKGROUND
	Color BGH = WHITE;	// HOVER

	bool activated = true;
	std::string id = "";
	std::string text = "";

	RectS Rect;
	PointerS pointer;
};