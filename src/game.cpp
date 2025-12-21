#include "game.hpp"

int BASE_WIDTH = 3840;
int BASE_HEIGHT = 2160;
float scale = 1.0f;
float offsetX = 0.0f;
float offsetY = 0.0f;
bool loadedResources = false;
// fix f11 and better single laser powerup handling
// ingrandire le sprite

Game::Game() 
    : GameStatus(std::make_shared<GameState>(START)), 
    MenuSystem(std::make_shared<Interface>(GameStatus)),
    Gamelayer(GameStatus, MenuSystem)
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Space Invaders");

    target = LoadRenderTexture(BASE_WIDTH, BASE_HEIGHT);
    
    scale = std::min((float)GetScreenWidth() / BASE_WIDTH, (float)GetScreenHeight() / BASE_HEIGHT);
    offsetX = (GetScreenWidth() - BASE_WIDTH * scale) * 0.5f;
    offsetY = (GetScreenHeight() - BASE_HEIGHT * scale) * 0.5f;

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
    if (!loadedResources)
    {
        GameCursor = AssetsManager::GetTexture("cursor");
        GameMusic = AssetsManager::GetMusic("game_music");
        loadedResources = true;
    }
}

void Game::Run()
{
    PlayMusicStream(GameMusic);
    HideCursor();

    while (!WindowShouldClose())
    {
        BeginTextureMode(target);
        ClearBackground(Color{58, 46, 63, 255});

        UpdateMusicStream(GameMusic);

        Gamelayer.UpdateGameStatus(GetFrameTime());
        MenuSystem->UpdateSystem();

        AudioManager();

        if (*GameStatus != RUNNING)
            DrawTexture(GameCursor, (GetMousePosition().x - offsetX) / scale - 10, (GetMousePosition().y - offsetY) / scale, WHITE);

        if (MenuSystem->WantToQuit())
            break;

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(target.texture, Rectangle{0, 0, (float)BASE_WIDTH, -(float)BASE_HEIGHT},
                       Rectangle{offsetX, offsetY, static_cast<float>(BASE_WIDTH * scale), static_cast<float>(BASE_HEIGHT * scale)}, Vector2{0, 0}, 0.0f,
                       WHITE);

        EndDrawing();
    }
    
    SaveCommands();
    CloseWindow();
}

void Game::AudioManager()
{
    SetMusicVolume(GameMusic,   (MenuSystem->GetMusicVolume() / 100)      * MenuSystem->GetGeneralVolume());

    Explosion::volume =        (MenuSystem->GetExplosionVolume() / 100)  * MenuSystem->GetGeneralVolume();
    Laser::volume =             (MenuSystem->GecooldownTimerLaserVolume() / 100)      * MenuSystem->GetGeneralVolume();
    Meteor::volume =         (MenuSystem->GetMeteorDamageVolume() / 100)   * MenuSystem->GetGeneralVolume();
    PowerUp::volume =           (MenuSystem->GetPowerUpVolume() / 100)    * MenuSystem->GetGeneralVolume();
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
    PowerUp::Init();
    Heart::Init();
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
    JsonParser p = JsonParser(PATH_SPACEINVADERS_SETTINGS);

    p.SetKey("audio", "GeneralVolume", MenuSystem->GetGeneralVolume());
    p.SetKey("audio", "MusicVolume", MenuSystem->GetMusicVolume());
    p.SetKey("audio", "LaserVolume", MenuSystem->GecooldownTimerLaserVolume());
    p.SetKey("audio", "MeteorDamageVolume", MenuSystem->GetMeteorDamageVolume());
    p.SetKey("audio", "PowerUpVolume", MenuSystem->GetPowerUpVolume());
    p.SetKey("audio", "ExplosionVolume", MenuSystem->GetExplosionVolume());

    p.SetKey("difficulty", "GameDifficulty", MenuSystem->GetGameDifficulty());

    p.SetKey("KeyBindings", "MOVEUP", KeyBinds.KeyUP);
    p.SetKey("KeyBindings", "MOVEDOWN", KeyBinds.KeyDOWN);
    p.SetKey("KeyBindings", "MOVELEFT", KeyBinds.KeyLEFT);
    p.SetKey("KeyBindings", "MOVERIGHT", KeyBinds.KeyRIGHT);
    p.SetKey("KeyBindings", "SHOOT", KeyBinds.KeySHOOT);

    p.SetKey("video", "Fullscreen", MenuSystem->GetFullscreen());

    p.SaveData();
}