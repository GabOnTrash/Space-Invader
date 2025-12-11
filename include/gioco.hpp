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

    GameState GameStatus = START;
    Music GameMusic;
    GameLayer Gamelayer;
    Interface MenuSystem;

    static inline Texture2D GameCursor = { 0 };
    static inline bool textureCaricata = false;
};