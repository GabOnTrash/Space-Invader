#pragma once
#include <memory>

#include "../SettingsManager/SettingsManager.hpp"
#include "../../../cmake-build-release/_deps/raylib-src/src/raylib.h"
#include "../IGameMode/IGameMode.hpp"
#include "../Renderer/Renderer.hpp"
#include "../MenuHandle/MenuHandle.hpp"
#include "../AudioManager/AudioManager.hpp"
#include "../stars.hpp"

struct GameContext
{
    int BASE_WIDTH;
    int BASE_HEIGHT;

    std::shared_ptr<GameState> gameStatus;
    Difficulty& difficulty;
    KeyBindings& keyBindings;
};

class Game
{
public:
    Game();
    ~Game();
    void Run();

private:
    void Update();
    void CreateContext();
    void LoadSinglePlayerMode();
    void LoadMultiPlayerMode();
    void SetUICallBacks();
    void DrawBackGround();


    Renderer renderer;
    KeyBindings keyBindings;
    Difficulty difficulty;
    std::shared_ptr<GameState> gameStatus;
    std::shared_ptr<MenuHandle> menuHandle;
    std::unique_ptr<IGameMode> currentMode;

    GameContext gameContext;

    Star backgroundStars;
    Music gameMusic;
    Texture2D gameCursor;
};
