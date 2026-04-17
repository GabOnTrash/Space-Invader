#pragma once

#include "Widget.h"
#include <functional>

template <typename T>
class Label : public Widget
{
public:
	Label() {}

	Label(std::string id, std::string text, std::function<T()> func, Font customFont, int fontS, float xPosCenter, float yPosCenter, Color FG, Color FGH)
	: id(id), text(text), function(func), customFont(customFont), fontS(fontS), posX(xPosCenter), posY(yPosCenter), FG(FG), FGH(FGH)
	{
		textSize = MeasureTextEx(customFont, text.c_str(), static_cast<float>(fontS), 1);

		posX = xPosCenter - (textSize.x / 2);
		posY = yPosCenter - (textSize.y / 2);
	}
	~Label() override {};

	void draw() override
	{
		if (function)
			function();

		if (hovered()) hover(FGH);
		else hover(FG);
	}
	bool isHovered() override
	{
		return hovered();
	}
	bool OnClick() override
	{
		return false;
	}

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
		Vector2 newSize = MeasureTextEx(customFont, newText.c_str(), static_cast<float>(fontS), 1);
		textSize = MeasureTextEx(customFont, text.c_str(), static_cast<float>(fontS), 1);
		text = newText;

		Vector2 difference;
		difference.x = newSize.x - textSize.x;
		difference.y = newSize.y - textSize.y;

		posX = posX - (difference.x / 2);
		posY = posY - (difference.y / 2);
	}
	void setActive(bool val) override { activated = val; }

	void setPosX(float value) override { posX = value; }
	void setPosY(float value) override { posY = value; }

private:
	std::string id = "";
	std::string text = "";
	
	std::function<T()> function;
	int fontS = 0;
	float posX = 0;
	float posY = 0;

	bool activated = true;

	void hover(Color color)
	{
		if constexpr (!std::is_same_v<void, T>)
		{
			if (function)
				DrawTextEx(customFont, TextFormat(text.c_str(), function()), { posX, posY }, static_cast<float>(fontS), 1, color);
		}
		else
			DrawTextEx(customFont, text.c_str(), { posX, posY }, static_cast<float>(fontS), 1, color);
	};
	bool hovered()
	{
		mousePos = renderer->GetVirtualMouse();
		return (mousePos.x >= posX && mousePos.x <= posX + textSize.x && mousePos.y >= posY && mousePos.y <= posY + textSize.y);
	}

	Font customFont;

	Color FG = WHITE;
	Color FGH = WHITE;

	Vector2 mousePos{ 0, 0 };
	Vector2 textSize{ 0, 0 };
};