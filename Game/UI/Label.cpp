#include "Label.h"

Label::Label(std::string id, std::string text, std::function<int()> func, Font customFont, int fontS, float xPosCenter, float yPosCenter, Color FG, Color FGH)
	: id(id), text(text), function(func), customFont(customFont), fontS(fontS), posX(xPosCenter), posY(yPosCenter), FG(FG), FGH(FGH)
{
	textSize = MeasureTextEx(customFont, text.c_str(), static_cast<float>(fontS), 1);

	posX = xPosCenter - (textSize.x / 2);
	posY = yPosCenter - (textSize.y / 2);
}
bool Label::OnClick()
{
    // not implemented for Label
	return false;
}
bool Label::hovered()
{
	mousePos = renderer->GetVirtualMouse();
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
	if (function != nullptr) DrawTextEx(customFont, TextFormat(text.c_str(), function()), { posX, posY }, static_cast<float>(fontS), 1, color);
	else DrawTextEx(customFont, text.c_str(), { posX, posY }, static_cast<float>(fontS), 1, color);
}