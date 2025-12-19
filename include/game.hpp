#pragma once

#include "interface.hpp"
#include "gamelayer.hpp"

#include "specs.hpp"
#include "JsonParser.hpp"

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
    std::shared_ptr<Interface> MenuSystem;
    GameLayer Gamelayer;

    Music GameMusic;
    static inline Texture2D GameCursor = { 0 };
};