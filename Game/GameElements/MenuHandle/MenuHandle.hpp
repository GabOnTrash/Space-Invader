#pragma once

#include "../Heart/Heart.hpp"
#include "../util/enums.hpp"

#include "../util/specs.hpp"
#include "../util/menuHandler.hpp"
#include "../Renderer/Renderer.hpp"

#include "../../UI/Menu.h"

class MenuHandle
{
public:
    MenuHandle();
    MenuHandle(GameContext& gameContext);

    void Update();
	void Draw();

	bool IsFullscreen();
	void UpdateDifficulty();
	void SetMenuSinglePlayer();
	void SetMenuMultiPlayer();
	bool UserWantsToQuit()
	{
		return shouldQuit;
	}
	std::function<void()> StartSinglePlayer;
	std::function<void()> StartMultiPlayer;
	std::function<void()> BackToMainMenu;
	std::function<void()> Restart;
	std::function<int()> GetScore;

	bool GameShouldUpdate = true;
	int DiffPerModifiers = 0;
	int nMaxHearts = 0;
	std::vector<Heart> heartsArray;
	Difficulty GameDifficulty = NORMAL;

private:
    void LoadGameFont();
	void ResumeGame();
	void InitAllMenus();
	void InitStartMenu();
	void ShowCountDown();
	void InitPausedMenu();
	void InitRunOverlay();
	void InitAudioControl();
	void InitBindsControls();
    void GoBackToMain();
    void UpdateSinglePlayerOverlay();
	void UpdateMultiPlayerOverlay();
	void HandleStateChange(GameState newState);
	void InitLinksIcons();
	void DrawLinksIcons();

	MenuHandler MainMenuHandler;
	std::shared_ptr<Menu> StartMenu;
	std::shared_ptr<Menu> RunningMenu;
	std::shared_ptr<Menu> PausedMenu;
	std::shared_ptr<Menu> AudioMenu;
	std::shared_ptr<Menu> ControlsMenu;
	std::shared_ptr<Menu> PreviousMenu;

	Timer<> timerDelayResume;

	bool isSinglePlayer = true;

	GameContext& gameContext;
	Texture2D instagram;
	Texture2D github;
	/////////////////////////////////

    const char* TranslateToDifficulty() const;
    void SetDifficulty();

private:
    void RecalculateLayout();

    void ChangeDifficulty();
    void setToBind(const std::string& id);
    void updateKeyBinding();
    const char* TranslateKey(int key);

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

    Color GrigioChiaro = Color{ 65, 65, 65, 255 };
    Color GrigioScuro = Color{ 30, 30, 30, 255 };
    Color Background = Color{ 58, 46, 63, 255 };
    Color transparent = Color{ 0, 0, 0, 0 };
    Color white = Color{ 255, 255, 255, 255 };

#define TRANSPARENT_COLOR transparent
#define FILL_COLOR_NHOVER GrigioScuro
#define FILL_COLOR_HOVER GrigioChiaro
#define TEXT_COLOR_NHOVER white
};

