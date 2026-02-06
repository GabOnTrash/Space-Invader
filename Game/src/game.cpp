#include "game.hpp"

namespace ViewPort
{
    int BASE_WIDTH = 1280;
    int BASE_HEIGHT = 720;
    float scale = 1.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    bool loadedResources = false;
}

Game::Game() 
    : GameStatus(std::make_shared<GameState>(START)), 
    MenuSystem(std::make_shared<MenuHandle>(GameStatus)),
    Gamelayer(GameStatus, MenuSystem)
{
    //SetConfigFlags(FLAG_WINDOW_UNDECORATED);


	SetExitKey(KEY_NULL);

    AssetsManager::Init(PATH_ASSETS_CONFIG);
    LoadAssets();

    InitUI();
    Gamelayer.SetDiff();
    
}
Game::~Game()
{
}

void Game::Init()
{
    if (!ViewPort::loadedResources)
    {
        GameCursor = AssetsManager::GetTexture("cursor");
        GameMusic = AssetsManager::GetMusic("game_music");
        ViewPort::loadedResources = true;
    }
}

void Game::Run()
{
    PlayMusicStream(GameMusic);
    HideCursor();

    while (!WindowShouldClose())
    {
        BeginTextureMode(target);
        ClearBackground(Color{18, 24, 38, 255});

        UpdateMusicStream(GameMusic);

        Gamelayer.UpdateSystem();
        MenuSystem->UpdateSystem();

        if (*GameStatus != RUNNING)
        {
            DrawTexture(GameCursor, static_cast<int>((GetMousePosition().x - ViewPort::offsetX) / ViewPort::scale - 10),
                        static_cast<int>((GetMousePosition().y - ViewPort::offsetY) / ViewPort::scale), WHITE);
            
            AudioManager();
        }

        if (MenuSystem->WantToQuit())
            break;

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(target.texture, Rectangle{0, 0, (float)ViewPort::BASE_WIDTH, -(float)ViewPort::BASE_HEIGHT},
                       Rectangle{ViewPort::offsetX, ViewPort::offsetY, static_cast<float>(ViewPort::BASE_WIDTH * ViewPort::scale), static_cast<float>(ViewPort::BASE_HEIGHT * ViewPort::scale)}, Vector2{0, 0}, 0.0f,
                       WHITE);

        EndDrawing();
    }
    
    SaveCommands();
    CloseWindow();
}

void Game::AudioManager()
{
    SetMusicVolume(GameMusic,   (MenuSystem->GetMusicVolume() / 100)                * MenuSystem->GetGeneralVolume());

    Explosion::volume =         (MenuSystem->GetExplosionVolume() / 100)            * MenuSystem->GetGeneralVolume();
    Laser::volume =             (MenuSystem->GetcooldownTimerLaserVolume() / 100)   * MenuSystem->GetGeneralVolume();
    Meteor::volume =            (MenuSystem->GetMeteorDamageVolume() / 100)         * MenuSystem->GetGeneralVolume();
    Modifier::volume =           (MenuSystem->GetPowerUpVolume() / 100)              * MenuSystem->GetGeneralVolume();
}
void Game::LoadAssets()
{
    InitAudioDevice();

    Game::Init();
    Player::Init();
    Laser::Init();
    BigLaser::Init();
    Meteor::Init();
    Explosion::Init();
    Modifier::Init();
    Heart::Init();

    SinglePlayerMode::InitIcons();
}
void Game::InitUI()
{
    MenuSystem->CallStart = [this]() { Gamelayer.Start(); };
    MenuSystem->CallStartMulti = [this]() { Gamelayer.StartMulti(); };
    MenuSystem->CallRestart = [this]() { Gamelayer.Restart(); };
    MenuSystem->CallResume = [this]() { Gamelayer.Resume(); };
    MenuSystem->CallGetScore = [this]() { return Gamelayer.GetGameScore(); };
    MenuSystem->CallSetDiff = [this]() { Gamelayer.SetDiff(); };
    MenuSystem->InitLayers();
}
