#include "gioco.hpp"

int BASE_WIDTH = 3840;
int BASE_HEIGHT = 2160;
float scale = 1.0f;
float offsetX = 0.0f;
float offsetY = 0.0f;

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

    InitUI();
    Gamelayer.SetDiff();
    LoadAssets();
}
Game::~Game()
{
    Astronave::Unload();
    Laser::Unload();
    BigLaser::Unload();
    Asteroide::Unload();
    Esplosione::Unload();
    PowerUp::Unload();
    Cuore::Unload();
    Game::Unload();
}

void Game::Init()
{
    if (!textureCaricata)
    {
        GameCursor = LoadTexture("immagini/pointer.png");
        GameMusic = LoadMusicStream("immagini/game_music.mp3");
        textureCaricata = true;
    }
}
void Game::Unload()
{
    if (textureCaricata)
    {
        UnloadTexture(GameCursor);
        UnloadMusicStream(GameMusic);
        textureCaricata = false;
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

    SettingsManager("SpaceInvadersSettings.json", *MenuSystem).SaveData();
    CloseWindow();
}

void Game::AudioManager()
{
    SetMusicVolume(GameMusic,   (MenuSystem->GetMusicVolume() / 100)      * MenuSystem->GetGeneralVolume());

    Esplosione::volume =        (MenuSystem->GetExplosionVolume() / 100)  * MenuSystem->GetGeneralVolume();
    Laser::volume =             (MenuSystem->GetLaserVolume() / 100)      * MenuSystem->GetGeneralVolume();
    Asteroide::volume =         (MenuSystem->GetAsteroidVolume() / 100)   * MenuSystem->GetGeneralVolume();
    PowerUp::volume =           (MenuSystem->GetPowerUpVolume() / 100)    * MenuSystem->GetGeneralVolume();
}
void Game::LoadAssets()
{
    InitAudioDevice();

    Game::Init();
    Astronave::Init();
    Laser::Init();
    BigLaser::Init();
    Asteroide::Init();
    Esplosione::Init();
    PowerUp::Init();
    Cuore::Init();
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
