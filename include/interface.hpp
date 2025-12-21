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
	float GecooldownTimerLaserVolume() { return LaserVolume; }
	float GetPowerUpVolume() { return PowerUpVolume; }
	float GetMeteorDamageVolume() { return MeteorDamageVolume; }
	float GetExplosionVolume() { return ExplosionVolume; }
	float GetGeneralVolume() { return GeneralVolume; }
	Difficulty GetGameDifficulty() { return GameDifficulty; }

	std::function<void()> CallStart;
	std::function<void()> CallRestart;
	std::function<void()> CallResume;
    std::function<int()> CallGetScore;
	std::function<void()> CallSetDiff;

private:
    void RecalculateLayout();

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

    Color GrigioChiaro = Color{ 65, 65, 65, 255 };
    Color GrigioScuro = Color{ 30, 30, 30, 255 };
    Color Background = Color{ 58, 46, 63, 255 };
    Color transparent = Color{ 0, 0, 0, 0 };

    std::shared_ptr<GameState> GameStatus;
    Difficulty GameDifficulty = NORMAL;
};
