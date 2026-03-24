#include "TextBox.hpp"

TextBox::TextBox(std::string id, float x, float y, float width, float height, Font font, int fontSize, int maxLength, Color fg, Color bg, Color bd, Color focusedBd)
    : id(id), maxLength(maxLength), customFont(font), fontS(fontSize), focusedBD(focusedBd)
{
    this->posX = x - width / 2;
    this->posY = y - height / 2;
    this->width = width;
    this->height = height;
    this->FG = fg;
    this->BG = bg;
    this->BD = bd;
}

bool TextBox::isHovered()
{
    mousePos = renderer->GetVirtualMouse();
    return CheckCollisionPointRec(mousePos, { posX, posY, width, height });
}

bool TextBox::OnClick()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        isFocused = isHovered();
        return isFocused;
    }
    return false;
}

void TextBox::update()
{
    if (!activated) return;

    OnClick();

    if (isFocused)
    {
        framesCounter++;

        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (text.length() < maxLength))
            {
                text += (char) key;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && text.length() > 0)
        {
            text.pop_back();
            framesCounter = 0;
        }

        if (IsKeyDown(KEY_BACKSPACE))
        {
            if (framesCounter > 30 && framesCounter % 3 == 0 && text.length() > 0)
            {
                text.pop_back();
            }
        }
    }
    else
    {
        framesCounter = 0;
    }
}

void TextBox::draw()
{
    if (!activated) return;

    Color currentBorder = isFocused ? focusedBD : BD;

    DrawRectangle(static_cast<int>(posX), static_cast<int>(posY), static_cast<int>(width), static_cast<int>(height), BG);
    DrawRectangleLines(static_cast<int>(posX), static_cast<int>(posY), static_cast<int>(width), static_cast<int>(height), currentBorder);

    DrawTextEx(customFont, text.c_str(), { posX + 5, posY + (height / 2) - (fontS / 2) }, static_cast<float>(fontS), 1, FG);

    if (isFocused && ((framesCounter / 20) % 2 == 0))
    {
        Vector2 textSize = MeasureTextEx(customFont, text.c_str(), static_cast<float>(fontS), 1);
        DrawRectangle(static_cast<int>(posX + 5 + textSize.x + 2), static_cast<int>(posY + (height / 2) - (fontS / 2)), 2, fontS, FG);
    }
}