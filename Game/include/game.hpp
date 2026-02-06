#pragma once

#include "MenuHandle/MenuHandle.hpp"
#include "SinglePlayerMode/SinglePlayerMode.hpp"

#include "specs.hpp"

class Game
{
public:
    Game();
    ~Game();

    void Init();
    void Run();

private:
    void AudioManager();
    void LoadAssets();
    void InitUI();
    void SaveCommands();

    RenderTexture2D target;
    
    std::shared_ptr<GameState> GameStatus;
    std::shared_ptr<MenuHandle> MenuSystem;
    SinglePlayerMode Gamelayer;

    Music GameMusic;
    static inline Texture2D GameCursor = { 0 };
};