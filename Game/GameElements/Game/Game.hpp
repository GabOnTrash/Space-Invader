#pragma once
#include <memory>

#include "../SettingsManager/SettingsManager.hpp"
#include "../util/specs.hpp"
#include "../IGameMode/IGameMode.hpp"
#include "../Renderer/Renderer.hpp"
#include "../MenuHandle/MenuHandle.hpp"
#include "../AudioManager/AudioManager.hpp"
#include "../Stars/Stars.hpp"

class Game
{
public:
    Game();
    ~Game();
    void Run();

private:
    void Update();
    void LoadSinglePlayerMode();
    void LoadMultiPlayerMode();
    void SetUICallBacks();
    void DrawBackGround();

    Renderer renderer;
    KeyBindings keyBindings;

    std::shared_ptr<GameState> gameStatus;
    std::shared_ptr<MenuHandle> menuHandle;
    std::unique_ptr<IGameMode> currentMode;

    GameContext gameContext;

    Star backgroundStars;
    Music gameMusic;
    Texture2D gameCursor;
};
