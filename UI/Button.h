#pragma once

#include <functional>
#include "Widget.h"
#include <iostream>
class Button : public Widget
{
public:

	Button();
	Button(std::string id, std::string text, Font fontJ, int fontS, float width, float height, float xPosCenter, float yPosCenter, std::function<void()> func, float round, float segm, float thickness, Color FG, Color BG, Color BGH, Color BD, Color BDH);

	void draw() override;
	void update() override;
	bool OnClick() override;	// funzione di tipo void
	bool isHovered() override;

	// Getters	// manca il get/set per il fontS
	std::string getId() const override { return id; }
	std::string getText() const override { return text; }
	bool getActive() const override { return activated; }

	float getPosX() const override { return posX; }
	float getPosY() const override { return posY; }

	float getWidth() const override { return width; }
	float getHeight() const override { return height; }

	float getRoundness() const override { return roundness; }
	float getSegments() const override { return segments; }
	float getThickness() const override { return thickness; }

	virtual Color getBG() const override { return BG; }
	virtual Color getFG() const override { return FG; }
	virtual Color getBD() const override { return BD; }
	virtual Color getBDH() const override { return BDH; }
	virtual Color getBGH() const override { return BGH; }

	// Setters
	void setId(std::string newId) override { id = newId; }
	void setText(std::string newText) override 
	{ 
		text = newText; 
		textSize = MeasureTextEx(fontJ, text.c_str(), static_cast<float>(fontS), 1);
	}
	void setActive(bool val) override { activated = val; }

	void setPosX(float value) override { posX = value - (width / 2); }
	void setPosY(float value) override { posY = value - (height / 2); }

	void setWidth(float value) override { width = value; }
	void setHeight(float value) override { height = value; }

	void setRoundness(float value) override { roundness = value; }
	void setSegments(float value) override { segments = value; }
	void setThickness(float value) override { thickness = value; }

	void setBG(Color value) override { BG = value; }
	void setFG(Color value) override { FG = value; }
	void setBD(Color value) override { BD = value; }
	void setBDH(Color value) override { BDH = value; }
	void setBGH(Color value) override { BGH = value; }

private:

	Font fontJ{};
	Vector2 textSize{ 0, 0 };
	Color BG = WHITE;	// BACKGROUND
	Color FG = WHITE;	// TEXT/FG
	Color BD = WHITE;	// BORDER COLOR
	Color BGH = WHITE;	// HOVER
	Color BDH = WHITE;	// HOVER

	std::string text = "";
	bool activated = true;

	int fontS = 0;

	float roundness = 0;
	float thickness = 4;
	float segments = 10;

	int StdCenterX = 0;
	int StdCenterY = 0;

protected:

	std::string id = "";
	float posX = 0;
	float posY = 0;
	float width = 0;
	float height = 0;

	bool singleClick = true;

	bool hovered();

	Vector2 mousePos{ 0, 0 };
	std::function<void()> function = nullptr;
};

class ButtonT : public Button
{
public:

	ButtonT() {}
	ButtonT(float xPosCenter, float yPosCenter, const char* pathN, std::function<void()> func = nullptr, float scale = 1, const char* pathC = "");
	~ButtonT() {}

	void update() override;
	bool OnClick() override;
	void draw() override;

private:

	void load();
	const char* pathN = ""; // normal image
	const char* pathC = ""; // image on click

	Texture2D textureN = { 0, 0, 0, 0, 0 };
	Texture2D textureC = { 0, 0, 0, 0, 0 };

	Vector2 textureNPos{ 0, 0 };
	Vector2 textureCPos{ 0, 0 };

	int imageW = 0;
	int imageH = 0;
	float scale = 1;
	float originalX = 0;
	float originalY = 0;
};