#pragma once

#include "../Heart/Heart.hpp"
#include "../util/enums.hpp"

#include "../util/specs.hpp"
#include "../Renderer/Renderer.hpp"

#include "../../UI/Menu.h"
#include "../SettingsManager/SettingsManager.hpp"

struct AudioSetting
{
	std::string key;
	std::string labelId;
	const char* labelText;
	std::string sliderId;
	float* volumeVar;
};


class MenuHandle
{
public:
    MenuHandle();
    MenuHandle(GameContext& gameContext);

    void Update();
	void Draw();
	void SetSizeAndId(size_t size, uint16_t id) { this->MATCH_SIZE = size; this->ID = id; }

	std::string GetIP();
    std::string GetPort();

	bool IsFullscreen();
	void UpdateDifficulty();

    bool UserWantsToQuit()
	{
		return shouldQuit;
	}

    std::function<void()> StartSinglePlayer;
	std::function<void()> StartMultiPlayer;
	std::function<void()> Reset;

	bool stillTryingConnecting = true;
	bool wantToRestartSinglePlayer = false;
	bool GameShouldUpdate = true;
	int DiffPerModifiers = 0;
	int nMaxHearts = 0;
	std::vector<Heart> heartsArray;
	Difficulty GameDifficulty = NORMAL;
    uint16_t ID;
	size_t MATCH_SIZE;

private:
    void LoadGameFont();
	void ShowCountDown();
	void ResumeGame(GameState st);

	void InitAllMenus();
	void InitSinglePlayerHUD();
    void InitMultiPlayerHUD();

	void InitStartMenu();
	void InitPauseMenu();
    void InitDeathMenu();
    void InitAudioMenu();
	void InitBindsMenu();
	void InitConneMenu();

	void UpdateMultiPlayerHUD();
    void UpdateSinglePlayerHUD();
	void HandleStateChange(GameState newState);

	Menu* currentMenu;
	Menu startMenu;
	Menu pauseMenu;
	Menu deathMenu;
	Menu audioMenu;
	Menu controlsMenu;
	Menu connectionMenu;
	Menu singlePlayerHUD;
    Menu multiPlayerHUD;

	std::unordered_map<GameState, Menu&> menus;
	std::vector<AudioSetting> settings;

	int lastID = -1;
	int lastMatchSize = -1;

	Timer<> timerDelayResume;
	GameContext& gameContext;

	/////////////////////////////////

    std::string SetDifficulty(bool change = false);

    void RecalculateLayout();

    void setToBind(const std::string& id);
    void updateKeyBinding();

    void ToggleFullscreen();

    std::string waitingForKeyBind = "";

    bool shouldQuit = false;
    bool fullscreen = true;
    int taskbar = 60; // Approximate taskbar height

    float MusicVolume = 0.0f;
    float LaserVolume = 0.0f;
    float PowerUpVolume = 0.0f;
    float MeteorDamageVolume = 0.0f;
    float ExplosionVolume = 0.0f;
    float GeneralVolume = 0.0f;

    float offsetY = 0.0f;
    float centerY = 0.0f;
    float blockSpacing = 0.0f;
    float sliderOffset = 0.0f;
    float labelX = 0.0f;
    float sliderX = 0.0f;
    float sliderWidth = 0.0f;
    float sliderHeight = 0.0f;
    float pointerWidth = 0.0f;
    float borderRadius = 0.0f;
    float fontSize = 0.0f;
    float buttonHeight = 0.0f;
    float buttonWidth = 0.0f;
    float spacing = 0.0f;
    float totalHeight = 0.0f;
    float yStart = 0.0f;
    float centerX = 0.0f;

    RectS rect;
    PointerS pointer;

    Font GameFontMedium;
    Font GameFontSemiBold;

    Color Background = Color{ 58, 46, 63, 255 };

#define TRANSPARENT_COLOR Color{ 0, 0, 0, 0 }
#define FILL_COLOR_NHOVER Color{ 30, 30, 30, 255 }
#define FILL_COLOR_HOVER Color{ 65, 65, 65, 255 }
#define TEXT_COLOR_NHOVER Color{ 255, 255, 255, 255 }
};

