#include "Slider.h"

Slider::Slider(std::string id, RectS& rectangle, PointerS& point)
	: id(id), rect(rectangle), pointer(point)
{
	// Determina se lo slider è verticale o orizzontale
	vert = (rect.getHeight() > rect.getWidth());

	// Se il puntatore è rettangolare, dimezza il lato principale
	if (!pointer.isRect()) pointer.setSide1(pointer.getSide1() / 2);

	// Centro rettangolo
	rect.setCenterX(rect.getPosX());
	rect.setCenterY(rect.getPosY());

	// Calcola posizione angolo superiore sinistro del rettangolo
	rect.setPosX(rect.getPosX() - rect.getWidth() / 2);
	rect.setPosY(rect.getPosY() - rect.getHeight() / 2);

	// Calcola i limiti dello slider
	float minVal = rect.getMin();
	float maxVal = rect.getMax();
	float value = *(rect.getVar());

	// Normalizza il valore nel range [0,1]
	float t = (value - minVal) / (maxVal - minVal);
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;

	if (vert)
	{
		// Slider verticale: il valore alto è in alto (quindi invertiamo 1.0f - t)
		float minY = rect.getPosY() + pointer.getSide2() / 2;
		float maxY = rect.getPosY() + rect.getHeight() - pointer.getSide2() / 2;
		float centerY = minY + (1.0f - t) * (maxY - minY);

		pointer.setCenterX(rect.getCenterX());
		pointer.setCenterY(centerY);
		pointer.setPosX(pointer.getCenterX() - pointer.getSide1() / 2);
		pointer.setPosY(pointer.getCenterY() - pointer.getSide2() / 2);
	}
	else
	{
		// Slider orizzontale: valore basso a sinistra, alto a destra
		float minX = rect.getPosX() + pointer.getSide1() / 2;
		float maxX = rect.getPosX() + rect.getWidth() - pointer.getSide1() / 2;
		float centerX = minX + t * (maxX - minX);

		pointer.setCenterX(centerX);
		pointer.setCenterY(rect.getCenterY());
		pointer.setPosX(pointer.getCenterX() - pointer.getSide1() / 2);
		pointer.setPosY(pointer.getCenterY() - pointer.getSide2() / 2);
	}
}

void Slider::draw()
{
	if (activated)
	{
		DrawRectangleRounded({ rect.getPosX(), rect.getPosY(), rect.getWidth(), rect.getHeight() }, rect.getRoundness(), static_cast<int>(rect.getSegments()), rect.getColor());

		if (pointer.isRect()) DrawRectangleRounded({ pointer.getPosX(), pointer.getPosY(), pointer.getSide1(), pointer.getSide2() }, pointer.getRoundness(), pointer.getSegments(), pointer.getColor());
		else DrawCircle(pointer.getCenterX(), pointer.getCenterY(), pointer.getSide1(), pointer.getColor());
	}
}
void Slider::setPointerVert()
{
	mousePos = GetMousePosition();
	ButtonDown = Pressed();

	if (mousePos.y != pointer.getCenterY() && ButtonDown && clicked)
	{
		if (mousePos.y <= rect.getPosY() + pointer.getSide2() / 2) pointer.setCenterY(rect.getPosY() + pointer.getSide2() / 2);
		else if (mousePos.y >= rect.getPosY() + rect.getHeight() - pointer.getSide2() / 2) pointer.setCenterY(rect.getPosY() + rect.getHeight() - pointer.getSide2() / 2);
		else pointer.setCenterY(mousePos.y);
	}

	if (pointer.isRect()) pointer.setPosY(pointer.getCenterY() - (pointer.getSide2() / 2));
}
void Slider::setPointerHoriz()
{
	mousePos = GetMousePosition();
	ButtonDown = Pressed();

	if (mousePos.x != pointer.getCenterX() && ButtonDown && clicked)
	{
		if (mousePos.x <= rect.getPosX() + pointer.getSide1() / 2) pointer.setCenterX(rect.getPosX() + pointer.getSide1() / 2);
		else if (mousePos.x >= rect.getPosX() + rect.getWidth() - pointer.getSide1() / 2) pointer.setCenterX(rect.getPosX() + rect.getWidth() - pointer.getSide1() / 2);
		else pointer.setCenterX(mousePos.x);
	}

	if (pointer.isRect()) pointer.setPosX(pointer.getCenterX() - (pointer.getSide1() / 2));
}
bool Slider::Pressed()
{
	if (hoveredR() && IsMouseButtonPressed(0)) clicked = true;

	if (hoveredR() && IsMouseButtonDown(0) || IsMouseButtonDown(0) && ButtonDown) return true;
	else if (IsMouseButtonUp(0)) 
	{
		clicked = false;
		return false;
	}

	return false;
}
bool Slider::hoveredR()
{
	mousePos = GetMousePosition();
	return CheckCollisionPointRec(mousePos, { rect.getPosX(), rect.getPosY(), rect.getWidth(), rect.getHeight() });
}
bool Slider::hoveredP()
{
	mousePos = GetMousePosition();
	return CheckCollisionPointCircle(mousePos, { pointer.getPosX(), pointer.getPosY() }, pointer.getSide1());
}
void Slider::update()
{
	if (activated)
	{
		vert ? setPointerVert() : setPointerHoriz();
		*(rect.getVar()) = static_cast<float>(GetValue());
	}
}
bool Slider::isHovered()
{
	return (hoveredR() || hoveredP());
}
float Slider::GetValue()
{
	if (vert)
	{
		minY = rect.getPosY() + pointer.getSide2() / 2;
		maxY = rect.getPosY() + rect.getHeight() - pointer.getSide2() / 2;
		rangeY = maxY - minY;

		normalized = 1.0f - ((pointer.getCenterY() - minY) / rangeY);
	}
	else
	{
		minX = rect.getPosX() + pointer.getSide1() / 2;
		maxX = rect.getPosX() + rect.getWidth() - pointer.getSide1() / 2;
		rangeX = maxX - minX;

		normalized = (pointer.getCenterX() - minX) / rangeX;
	}

	*(rect.getVar()) = static_cast<float>(rect.getMin() + normalized * (rect.getMax() - rect.getMin()));
	return *(rect.getVar());
}