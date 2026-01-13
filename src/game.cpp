#include "game.hpp"

namespace ViewPort
{
    int BASE_WIDTH = 3840;
    int BASE_HEIGHT = 2160;
    float scale = 1.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    bool loadedResources = false;
}

Game::Game() 
    : GameStatus(std::make_shared<GameState>(START)), 
    MenuSystem(std::make_shared<MenuLayer>(GameStatus)),
    Gamelayer(GameStatus, MenuSystem)
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Space Invaders");

    target = LoadRenderTexture(ViewPort::BASE_WIDTH, ViewPort::BASE_HEIGHT);
    
    ViewPort::scale = std::min((float)GetScreenWidth() / ViewPort::BASE_WIDTH, (float)GetScreenHeight() / ViewPort::BASE_HEIGHT);
    ViewPort::offsetX = (GetScreenWidth() - ViewPort::BASE_WIDTH * ViewPort::scale) * 0.5f;
    ViewPort::offsetY = (GetScreenHeight() - ViewPort::BASE_HEIGHT * ViewPort::scale) * 0.5f;

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

    GameLayer::InitIcons();
}
void Game::InitUI()
{
    MenuSystem->CallStart = [this]() { Gamelayer.Start(); };
    MenuSystem->CallRestart = [this]() { Gamelayer.Restart(); };
    MenuSystem->CallResume = [this]() { Gamelayer.Resume(); };
    MenuSystem->CallGetScore = [this]() { return Gamelayer.GetGameScore(); };
    MenuSystem->CallSetDiff = [this]() { Gamelayer.SetDiff(); };
    MenuSystem->InitLayers();
}
void Game::SaveCommands()
{
    JsonParser p(PATH_SPACEINVADERS_SETTINGS);

    p.SetKey("audio", "GeneralVolume", MenuSystem->GetGeneralVolume());
    p.SetKey("audio", "MusicVolume", MenuSystem->GetMusicVolume());
    p.SetKey("audio", "LaserVolume", MenuSystem->GetcooldownTimerLaserVolume());
    p.SetKey("audio", "MeteorDamageVolume", MenuSystem->GetMeteorDamageVolume());
    p.SetKey("audio", "PowerUpVolume", MenuSystem->GetPowerUpVolume());
    p.SetKey("audio", "ExplosionVolume", MenuSystem->GetExplosionVolume());

    p.SetKey("difficulty", "GameDifficulty", MenuSystem->GetGameDifficulty());

    p.SetKey("KeyBindings", "MOVEUP", KeyBinds.KeyUP);
    p.SetKey("KeyBindings", "MOVEDOWN", KeyBinds.KeyDOWN);
    p.SetKey("KeyBindings", "MOVELEFT", KeyBinds.KeyLEFT);
    p.SetKey("KeyBindings", "MOVERIGHT", KeyBinds.KeyRIGHT);
    p.SetKey("KeyBindings", "SHOOT", KeyBinds.KeySHOOT);
    p.SetKey("KeyBindings", "DASH", KeyBinds.KeyDASH);

    p.SetKey("video", "Fullscreen", MenuSystem->GetFullscreen());

    p.SaveData();
}