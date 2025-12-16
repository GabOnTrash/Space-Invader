#pragma once

#include "interface.hpp"
#include "gamelayer.hpp"

#include "specs.hpp"
#include "settingsManager.hpp"

class Game
{
public:

    Game();
    ~Game();

    void Init();
    void Unload();
    void Run();

private:

    void AudioManager();
    void LoadAssets();

    void InitUI();

    
    std::shared_ptr<GameState> GameStatus;
    std::shared_ptr<Interface> MenuSystem;
    GameLayer Gamelayer;

    Music GameMusic;
    static inline Texture2D GameCursor = { 0 };
    static inline bool textureCaricata = false;
};