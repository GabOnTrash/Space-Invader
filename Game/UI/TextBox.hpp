#pragma once
#include "Widget.h"
#include <string>

class TextBox : public Widget
{
public:
    TextBox(std::string id, float x, float y, float width, float height, Font font, int fontSize, int maxLength, Color fg, Color bg, Color bd, Color focusedBd);

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

    Color focusedBD; // Colore del bordo quando selezionato
    bool isFocused = false;
    int framesCounter = 0; // Usato per il cursore lampeggiante e il backspace continuo

    Vector2 mousePos{ 0, 0 };
};