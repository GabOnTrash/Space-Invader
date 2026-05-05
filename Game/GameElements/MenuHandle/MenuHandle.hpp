#pragma once

#include "../Heart/Heart.hpp"
#include "../util/enums.hpp"
#include "../util/GameContext.hpp"
#include "../util/performance.hpp"
#include "../util/Strings.hpp"
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
	void SetSize(size_t size) { this->MATCH_SIZE = size; }
	void SetID(uint16_t id) { this->ID = id; }

	std::string GetIP();
    std::string GetPort();

	bool IsFullscreen();
	void UpdateDifficulty();
    void setCurrentScore(int gameScore);
	void RenderCountDown(int remainingSeconds);

    bool UserWantsToQuit()
	{
		return shouldQuit;
	}

    std::function<void()> StartSinglePlayer;
	std::function<void()> StartMultiPlayer;
	std::function<void()> Reset;

	bool shouldResumeTheGame = false;
	bool stillTryingConnecting = true;
	bool wantToRestartSinglePlayer = false;
	int DiffPerModifiers = 0;
	int nMaxHearts = 0;
	std::vector<Heart> heartsArray;
	Difficulty GameDifficulty = NORMAL;
    uint16_t ID;
	size_t MATCH_SIZE;

private:
    void LoadGameFont();
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

