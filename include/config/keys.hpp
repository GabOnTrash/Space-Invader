#pragma once

struct KeyBindings
{
    int KeyUP = KEY_W;
    int KeyDOWN = KEY_S;
    int KeyLEFT = KEY_A;
    int KeyRIGHT = KEY_D;
    int KeySHOOT = KEY_SPACE;
    int KeyDASH = KEY_LEFT_SHIFT;
};

static inline std::unordered_map<int, std::string> RaylibKeyToString = {
    {KEY_APOSTROPHE, "'"},
    {KEY_COMMA, ","},
    {KEY_MINUS, "-"},
    {KEY_PERIOD, "."},
    {KEY_SLASH, "/"},

    {KEY_ZERO, "0"},
    {KEY_ONE, "1"},
    {KEY_TWO, "2"},
    {KEY_THREE, "3"},
    {KEY_FOUR, "4"},
    {KEY_FIVE, "5"},
    {KEY_SIX, "6"},
    {KEY_SEVEN, "7"},
    {KEY_EIGHT, "8"},
    {KEY_NINE, "9"},

    {KEY_SEMICOLON, ";"},
    {KEY_EQUAL, "="},

    {KEY_A, "A"},
    {KEY_B, "B"},
    {KEY_C, "C"},
    {KEY_D, "D"},
    {KEY_E, "E"},
    {KEY_F, "F"},
    {KEY_G, "G"},
    {KEY_H, "H"},
    {KEY_I, "I"},
    {KEY_J, "J"},
    {KEY_K, "K"},
    {KEY_L, "L"},
    {KEY_M, "M"},
    {KEY_N, "N"},
    {KEY_O, "O"},
    {KEY_P, "P"},
    {KEY_Q, "Q"},
    {KEY_R, "R"},
    {KEY_S, "S"},
    {KEY_T, "T"},
    {KEY_U, "U"},
    {KEY_V, "V"},
    {KEY_W, "W"},
    {KEY_X, "X"},
    {KEY_Y, "Y"},
    {KEY_Z, "Z"},

    {KEY_LEFT_BRACKET, "["},
    {KEY_BACKSLASH, "\\"},
    {KEY_RIGHT_BRACKET, "]"},
    {KEY_GRAVE, "`"},

    {KEY_SPACE, "Space"},
    {KEY_ESCAPE, "Esc"},
    {KEY_ENTER, "Enter"},
    {KEY_TAB, "Tab"},
    {KEY_BACKSPACE, "Backspace"},

    {KEY_INSERT, "Insert"},
    {KEY_DELETE, "Delete"},

    {KEY_RIGHT, "Right Arrow"},
    {KEY_LEFT, "Left Arrow"},
    {KEY_DOWN, "Down Arrow"},
    {KEY_UP, "Up Arrow"},

    {KEY_PAGE_UP, "Page Up"},
    {KEY_PAGE_DOWN, "Page Down"},
    {KEY_HOME, "Home"},
    {KEY_END, "End"},

    {KEY_CAPS_LOCK, "Caps Lock"},
    {KEY_SCROLL_LOCK, "Scroll Lock"},
    {KEY_NUM_LOCK, "Num Lock"},
    {KEY_PRINT_SCREEN, "Print Screen"},
    {KEY_PAUSE, "Pause"},

    {KEY_F1, "F1"},
    {KEY_F2, "F2"},
    {KEY_F3, "F3"},
    {KEY_F4, "F4"},
    {KEY_F5, "F5"},
    {KEY_F6, "F6"},
    {KEY_F7, "F7"},
    {KEY_F8, "F8"},
    {KEY_F9, "F9"},
    {KEY_F10, "F10"},
    {KEY_F11, "F11"},
    {KEY_F12, "F12"},

    {KEY_LEFT_SHIFT, "Left Shift"},
    {KEY_LEFT_CONTROL, "Left Ctrl"},
    {KEY_LEFT_ALT, "Left Alt"},
    {KEY_LEFT_SUPER, "Left Super"},

    {KEY_RIGHT_SHIFT, "Right Shift"},
    {KEY_RIGHT_CONTROL, "Right Ctrl"},
    {KEY_RIGHT_ALT, "Right Alt"},
    {KEY_RIGHT_SUPER, "Right Super"},

    {KEY_KB_MENU, "Menu"},
};
