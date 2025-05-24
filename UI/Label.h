#pragma once

#include "Widget.h"
#include <functional>

class Label : public Widget
{
public:

	Label() {}
	Label(std::string id, std::string text, std::function<int()> func, Font fontJ, int fontS, float xPosCenter, float yPosCenter, Color FG, Color FGH);

	void draw() override;
	bool isHovered() override;
	bool OnClick() override;

	// Getters	// manca il get/set per il fontS e per function
	std::string getId() const override { return id; }
	std::string getText() const override { return text; }
	bool getActive() const override { return activated; }

	float getPosX() const override { return posX; }
	float getPosY() const override { return posY; }

	// Setters
	void setId(std::string newId) override { id = newId; }
	void setText(std::string newText) override 
	{ 
		text = newText;
		/*textSize = MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(fontS), 1); 

		posX = StdCenteredX - (textSize.x / 2);
		posY = StdCenteredY - (textSize.y / 2);*/
	}
	void setActive(bool val) override { activated = val; }

	void setPosX(float value) override { posX = value; }
	void setPosY(float value) override { posY = value; }

private:

	std::string id = "";
	std::string text = "";
	
	std::function<int()> function;

	int fontS = 0;
	float posX = 0;
	float posY = 0;

	//float StdCenteredX = 0;
	//float StdCenteredY = 0;

	bool activated = true;

	void hover(Color FGH);
	bool hovered();

	Font fontJ;

	Color FG = WHITE;
	Color FGH = WHITE;

	Vector2 mousePos{ 0, 0 };
	Vector2 textSize{ 0, 0 };
};