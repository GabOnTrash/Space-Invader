#include "Label.h"

Label::Label(std::string id, std::string text, std::function<int()> func, Font fontJ, int fontS, float xPosCenter, float yPosCenter, Color FG, Color FGH)
	: id(id), text(text), function(func), fontJ(fontJ), fontS(fontS), posX(xPosCenter), posY(yPosCenter), FG(FG), FGH(FGH)
{
	textSize = MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(fontS), 1);

	posX = xPosCenter - (textSize.x / 2);
	posY = yPosCenter - (textSize.y / 2);

	/*StdCenteredX = posX;
	StdCenteredY = posY;*/
}
bool Label::OnClick()
{
	/*if (hovered() && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && singleClick)
	{
		singleClick = false;

		if (function != nullptr)
			function();
		return true;
	}*/
	/*else singleClick = true;*/

	return false;
}
bool Label::hovered()
{
	mousePos = GetMousePosition();
	return (mousePos.x >= posX && mousePos.x <= posX + textSize.x && mousePos.y >= posY && mousePos.y <= posY + textSize.y);
}
void Label::draw()
{
	if (hovered()) hover(FGH);
	else hover(FG);
}
bool Label::isHovered()
{
	return hovered();
}
void Label::hover(Color color)
{
	if (function != nullptr) DrawTextEx(fontJ, TextFormat(text.c_str(), function()), { posX, posY }, static_cast<float>(fontS), 1, color);
	else DrawTextEx(fontJ, text.c_str(), { posX, posY }, static_cast<float>(fontS), 1, color);
}