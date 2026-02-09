#include "Button.h"

Renderer* Widget::renderer = nullptr;

Button::Button() {}
Button::Button(std::string id, std::string text, Font fontJ, int fontS, float width, float height, float xPosCenter, float yPosCenter, std::function<void()> func, float round, int segm, float thickness, Color FG, Color BG, Color BGH, Color BD, Color BDH)
	: id(id), text(text), customFont(fontJ), fontS(fontS), width(width), height(height), posX(xPosCenter - (width / 2)), posY(yPosCenter - (height / 2)), roundness(round), segments(segm), thickness(thickness), FG(FG), BG(BG), BGH(BGH), BD(BD), BDH(BDH)
{
	textSize = MeasureTextEx(fontJ, text.c_str(), static_cast<float>(fontS), 1);
	StdCenterX = static_cast<int>(posX + (width / 2) - (textSize.x / 2));
	StdCenterY = static_cast<int>(posY + (height / 2) - (textSize.y / 2));

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

	DrawTextEx(customFont, text.c_str(), { posX + (width / 2 - textSize.x / 2), posY + (height / 2 - textSize.y / 2) }, static_cast<float>(fontS), 1, FG);
}
bool Button::hovered()
{
    mousePos = renderer->GetVirtualMouse();
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