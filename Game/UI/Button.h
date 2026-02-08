#pragma once

#include <functional>
#include <iostream>
#include "Widget.h"
class Button : public Widget
{
public:
	Button();
	Button(std::string id, std::string text, Font fontJ, int fontS, float width, float height, float xPosCenter, float yPosCenter, std::function<void()> func, float round, int segm, float thickness, Color FG, Color BG, Color BGH, Color BD, Color BDH);
	
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
		textSize = MeasureTextEx(customFont, text.c_str(), static_cast<float>(fontS), 1);
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
	Font customFont{};
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
	int segments = 10;

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

class TextureButton : public Widget
{
public:
	TextureButton() {}
	TextureButton(float xPosCenter, float yPosCenter,
		Texture2D& texture, std::function<void()> func = nullptr)
			: texture(texture)
	{
		position.x = xPosCenter - (texture.width / 2);
		position.y = yPosCenter - (texture.height / 2);
		this->func = func;
	}
	virtual ~TextureButton() {}

	void Clicked()
	{
		if (/*hovered() && */IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && singleClick)
		{
			singleClick = false;

			if (func != nullptr)
				func();
		}
		else
			singleClick = true;
	}
	void update() override
	{
		Clicked();
	}
	void draw() override
	{
		DrawTexture(texture, position.x, position.y, WHITE);
	}

private:
	/*bool TextureButton::hovered()
	{
		Vector2 mousePos = GetMousePosition();
		mousePos.x = (mousePos.x - co offsetX) / ViewPort::scale;
		mousePos.y = (mousePos.y - ViewPort::offsetY) / ViewPort::scale;

		return (mousePos.x >= posX && mousePos.x <= posX + width && mousePos.y >= posY && mousePos.y <= posY + height);
	}*/

	Texture2D texture{ 0 };
	Vector2 position{ 0, 0 };
	std::function<void()> func;
	bool singleClick = false;
};