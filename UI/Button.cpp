#include "Button.h"

Button::Button() {}
Button::Button(std::string id, std::string text, Font fontJ, int fontS, float width, float height, float xPosCenter, float yPosCenter, std::function<void()> func, float round, float segm, float thickness, Color FG, Color BG, Color BGH, Color BD, Color BDH)
	: id(id), text(text), fontJ(fontJ), fontS(fontS), width(width), height(height), posX(xPosCenter - (width / 2)), posY(yPosCenter - (height / 2)), roundness(round), segments(segm), thickness(thickness), FG(FG), BG(BG), BGH(BGH), BD(BD), BDH(BDH)
{
	textSize = MeasureTextEx(fontJ, text.c_str(), static_cast<float>(fontS), 1);
	StdCenterX = posX + (width / 2) - (textSize.x / 2);
	StdCenterY = posY + (height / 2) - (textSize.y / 2);

	this->function = func;
}
void Button::draw()
{
	if (hovered())
	{
		DrawRectangleRoundedLines(Rectangle{ posX, posY, width, height }, roundness, static_cast<int>(segments), thickness, BDH);
		DrawRectangleRounded(Rectangle{ posX, posY, width, height }, roundness, static_cast<int>(segments), BGH);
	}
	else
	{
		DrawRectangleRoundedLines(Rectangle{ posX, posY, width, height }, roundness, static_cast<int>(segments), thickness, BD);
		DrawRectangleRounded(Rectangle{ posX, posY, width, height }, roundness, static_cast<int>(segments), BG);
	}

	DrawTextEx(fontJ, text.c_str(), { posX + (width / 2 - textSize.x / 2), posY + (height / 2 - textSize.y / 2) }, static_cast<float>(fontS), 1, FG);
}
bool Button::hovered()
{
	mousePos = GetMousePosition();
	return (mousePos.x >= posX && mousePos.x <= posX + width && mousePos.y >= posY && mousePos.y <= posY + height);
}
void Button::update()
{
	OnClick();
}
bool Button::OnClick()
{
	if (hovered() && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && singleClick)
	{
		singleClick = false;

		if (function != nullptr)
			function();
		return true;
	}
	else singleClick = true;

	return false;
}
bool Button::isHovered()
{
	return hovered();
}

ButtonT::ButtonT(float xPosCenter, float yPosCenter, const char* pathN, std::function<void()> func, float scale, const char* pathC)
{
	posX = xPosCenter;
	posY = yPosCenter;

	this->pathN = pathN;
	this->pathC = pathC;
	this->scale = scale;

	this->function = func;

	load();
}
void ButtonT::load()
{
	Image image = LoadImage(pathN);

	imageW = image.width;
	imageH = image.height;

	width = static_cast<float>(imageW * scale);
	height = static_cast<float>(imageH * scale);

	ImageResize(&image, static_cast<int>(width), static_cast<int>(height));
	textureN = LoadTextureFromImage(image);
	UnloadImage(image);

	originalX = posX; // Memorizza la posizione originale
	originalY = posY;

	posX = (posX - (width / 2));
	posY = (posY - (height / 2));
	textureNPos = { posX, posY };

	//// if imageC
	if (pathC != "")
	{
		image = LoadImage(pathC);

		imageW = image.width;
		imageH = image.height;

		width = static_cast<float>(imageW * scale);
		height = static_cast<float>(imageH * scale);

		ImageResize(&image, static_cast<int>(width), static_cast<int>(height));
		textureC = LoadTextureFromImage(image);
		UnloadImage(image);

		posX = (originalX - (width / 2));
		posY = (originalY - (height / 2));
		textureCPos = { posX, posY };
	}
}
void ButtonT::update()
{
	OnClick();
}
bool ButtonT::OnClick()
{
	if (hovered() && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && singleClick)
	{
		singleClick = false;

		if (function != nullptr)
			function();

		return true;
	}
	else singleClick = false;
	return false;
}
void ButtonT::draw()
{
	if (hovered() && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && pathC != "") DrawTextureV(textureC, textureCPos, WHITE);
	else DrawTextureV(textureN, textureNPos, WHITE);
}