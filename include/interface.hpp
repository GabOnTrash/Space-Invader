#pragma once

#include "specs.hpp"
#include "menuHandler.hpp"

class Interface
{
public:

    Interface() = default;
    Interface(std::shared_ptr<GameState> GameStatus);

    void InitLayers();
    void UpdateSystem();

    const char* TranslateToDifficulty();
    void SetDifficulty();

    void SetLayerGame();

    bool WantToQuit()
    {
        return shouldQuit;
    }

    std::shared_ptr<Menu> GetRunningMenu();
    std::shared_ptr<Menu> GetStartMenu();
    std::shared_ptr<Menu> GetPausedMenu();
    std::shared_ptr<Menu> GetAudioMenu();
    std::shared_ptr<Menu> GetControlsMenu();

    bool GetFullscreen() { return fullscreen; }
	float GetMusicVolume() { return MusicVolume; }
	float GetLaserVolume() { return LaserVolume; }
	float GetPowerUpVolume() { return PowerUpVolume; }
	float GetAsteroidVolume() { return AsteroidVolume; }
	float GetExplosionVolume() { return ExplosionVolume; }
	float GetGeneralVolume() { return GeneralVolume; }
	Difficulty GetGameDifficulty() { return GameDifficulty; }

	std::function<void()> CallStart;
	std::function<void()> CallRestart;
	std::function<void()> CallResume;
    std::function<int()> CallGetScore;
	std::function<void()> CallSetDiff;

private:

    void ChangeDifficulty();
    void setToBind(const std::string& id);
    void updateKeyBinding();
    const char* TranslateKey(int key);

    void InitStartMenuSettings();
    void InitRunningOverlay();
    void InitPausedMenuSettings();
    void InitAudioControlSettings();
    void InitControlsSettings();
    
    void SetLayerStart();
    void SetLayerPausedMenu(bool dead = false);
    void SetLayerControls();
    void SetLayerAudio();

    void ToggleFullscreen();

    std::string waitingForKeyBind = "";

    bool shouldQuit = false;
    bool fullscreen = true;
    int taskbar = 60; // Approximate taskbar height

    float MusicVolume = 0.0f;
    float LaserVolume = 0.0f;
    float PowerUpVolume = 0.0f;
    float AsteroidVolume = 0.0f;
    float ExplosionVolume = 0.0f;
    float GeneralVolume = 0.0f;
    int offsetY = 100;

    int centerY = WINDOW_HEIGHT / 2;
    int blockSpacing = 140 * SCALE;
    int sliderOffset = 50 * SCALE;
    int labelX = WINDOW_WIDTH / 2;
    int sliderX = WINDOW_WIDTH / 2;
    int sliderWidth = 400 * SCALE;
    int sliderHeight = 60 * SCALE;
    int pointerWidth = 20 * SCALE;

    float fontSize = 50 * SCALE;
    float buttonHeight = 120 * SCALE;
    float spacing = 90 * SCALE;
    float totalHeight = (buttonHeight * 6) + (spacing * 5);
    float yStart = (WINDOW_HEIGHT / 2) - (totalHeight / 2);
    float centerX = WINDOW_WIDTH / 2;

    MenuHandler MainMenuHandler;

    RectS rect;
    PointerS pointer;

    std::shared_ptr<Menu> StartMenu;
    std::shared_ptr<Menu> RunningMenu;
    std::shared_ptr<Menu> PausedMenu;
    std::shared_ptr<Menu> AudioMenu;
    std::shared_ptr<Menu> ControlsMenu;
    std::shared_ptr<Menu> PreviousMenu;

    Font GameFont;
    Strings text;

    Color GrigioChiaro = Color{ 65, 65, 65, 255 };
    Color GrigioScuro = Color{ 30, 30, 30, 255 };
    Color Background = Color{ 58, 46, 63, 255 };
    Color transparent = Color{ 0, 0, 0, 0 };

    std::shared_ptr<GameState> GameStatus;
    Difficulty GameDifficulty = NORMAL;
};
