#pragma once
#include "Widget.h"
#include <string>

class TextBox : public Widget
{
public:
    TextBox(std::string id, float x, float y, float width, float height, Font font, int fontSize, int maxLength, int minRange, int maxRange, Color fg, Color bg, Color bd, Color focusedBd);
    ~TextBox() override {};

    void draw() override;
    void update() override;
    bool OnClick() override;
    bool isHovered() override;

    // Getters / Setters
    std::string getId() const override { return id; }
    std::string getText() const override { return text; }
    void setText(std::string newText) override { text = newText; }

    bool getFocused() const { return isFocused; }
    void setFocused(bool focused) { isFocused = focused; }

private:
    std::string id;
    std::string text = "";
    int maxLength;

    Font customFont;
    int fontS;

    Color focusedBD;
    bool isFocused = false;
    int framesCounter = 0;
    int min, max;

    Vector2 mousePos{ 0, 0 };
};