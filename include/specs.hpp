#pragma once

#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <stack>
#include <memory>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <raylib.h>
#include <raymath.h>

#include "timer.hpp"
#include "MyRandom.hpp"
#include "Menu.h"


extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern float SCALE;

struct KeyBindings
{
    int KeyUP = KEY_W;
    int KeyDOWN = KEY_S;
    int KeyLEFT = KEY_A;
    int KeyRIGHT = KEY_D;
    int KeySHOOT = KEY_SPACE;
};

extern KeyBindings KeyBinds;

enum GameState
{
    START = 0,
    RUNNING = 1,
    PAUSED = 2,
    KILLED = 3,
};

enum MeteorState
{
	INTEGRITY = 0,
	DAMAGED = 1,
	DESTRYED = 2
};

enum Difficulty
{
    EASY = 5,
    NORMAL = 4,
    HARD = 3
};

struct Strings
{
    static inline const char* title = "Space invader";
    static inline const char* quit = "Quit";
    static inline const char* restart = "Restart";
    static inline const char* resume = "Resume";

    static inline const char* ready = "Ready";
    static inline const char* start = "Start the game";

    static inline const char* difficulty = "Difficulty: %s";
    static inline const char* score = "Score: %d";
    static inline const char* easy = "Easy";
    static inline const char* normal = "Normal";
    static inline const char* hard = "Hard";
    static inline const char* danger = "Danger!";

    static inline const char* audioSettings = "Audio";
    static inline const char* GENVOLUME = "General Volume";
    static inline const char* MUSCVOLUME = "BG Music Volume";
    static inline const char* LASVOLUME = "Lasers Volume";
    static inline const char* ASTVOLUME = "Asteroids Volume";
    static inline const char* EXPLVOLUME = "Explosions Volume";
    static inline const char* PWVOLUME = "PowerUps Volume";

    static inline const char* control = "Control";
	static inline const char* moveup = "UP: %s";
	static inline const char* movedown = "DOWN: %s";
	static inline const char* moveleft = "LEFT: %s";
	static inline const char* moveright = "RIGHT: %s";
	static inline const char* shoot = "SHOOT: %s";
    static inline const char* reset = "Reset bindings";

    // static inline const char* InUse = "Key in use.";
    static inline const char* waitingKey = "Press a key...";

    static inline const char* notFound = "Res not found.";
};

static inline std::unordered_map<int, std::string> RaylibKeyToString =
{
    { KEY_APOSTROPHE, "'" },
    { KEY_COMMA, "," },
    { KEY_MINUS, "-" },
    { KEY_PERIOD, "." },
    { KEY_SLASH, "/" },
    { KEY_ZERO, "0" },
    { KEY_ONE, "1" },
    { KEY_TWO, "2" },
    { KEY_THREE, "3" },
    { KEY_FOUR, "4" },
    { KEY_FIVE, "5" },
    { KEY_SIX, "6" },
    { KEY_SEVEN, "7" },
    { KEY_EIGHT, "8" },
    { KEY_NINE, "9" },
    { KEY_SEMICOLON, ";" },
    { KEY_EQUAL, "=" },
    { KEY_A, "A" },
    { KEY_B, "B" },
    { KEY_C, "C" },
    { KEY_D, "D" },
    { KEY_E, "E" },
    { KEY_F, "F" },
    { KEY_G, "G" },
    { KEY_H, "H" },
    { KEY_I, "I" },
    { KEY_J, "J" },
    { KEY_K, "K" },
    { KEY_L, "L" },
    { KEY_M, "M" },
    { KEY_N, "N" },
    { KEY_O, "O" },
    { KEY_P, "P" },
    { KEY_Q, "Q" },
    { KEY_R, "R" },
    { KEY_S, "S" },
    { KEY_T, "T" },
    { KEY_U, "U" },
    { KEY_V, "V" },
    { KEY_W, "W" },
    { KEY_X, "X" },
    { KEY_Y, "Y" },
    { KEY_Z, "Z" },
    { KEY_LEFT_BRACKET, "["},
    { KEY_BACKSLASH, "\\"},
    { KEY_RIGHT_BRACKET, "]"},
    { KEY_GRAVE, "`"},
	{ KEY_SPACE, "SPACE" },
	{ KEY_ESCAPE, "ESCAPE" },
	{ KEY_ENTER, "ENTER" },
	{ KEY_TAB, "TAB" },
	{ KEY_BACKSPACE, "BACKSPACE" },
	{ KEY_INSERT, "INSERT" },
	{ KEY_DELETE, "DELETE" },
	{ KEY_RIGHT, "RIGHT" },
	{ KEY_LEFT, "LEFT" },
	{ KEY_DOWN, "DOWN" },
	{ KEY_UP, "UP" },
	{ KEY_PAGE_UP, "PAGE UP" },
	{ KEY_PAGE_DOWN, "PAGE DOWN" },
	{ KEY_HOME, "HOME" },
	{ KEY_END, "END" },
	{ KEY_CAPS_LOCK, "CAPS LOCK" },
	{ KEY_SCROLL_LOCK, "SCROLL LOCK" },
	{ KEY_NUM_LOCK, "NUM LOCK" },
	{ KEY_PRINT_SCREEN, "PRINT SCREEN" },
};
