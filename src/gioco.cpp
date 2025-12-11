#include "gioco.hpp"

int WINDOW_WIDTH = 1500;
int WINDOW_HEIGHT = 1500;
float SCALE = 1.0f;

Game::Game()
    : MenuSystem(&GameStatus),
      Gamelayer(&GameStatus, &MenuSystem)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Invaders");

    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);

    SCALE = std::min((float) screenWidth / 3840.0f, 1.0f);

    WINDOW_WIDTH = static_cast<int>(WINDOW_WIDTH * SCALE);
    WINDOW_HEIGHT = static_cast<int>(WINDOW_HEIGHT * SCALE);

	SetWindowPosition((screenWidth - WINDOW_WIDTH) / 2, (screenHeight - WINDOW_HEIGHT) / 2);
    SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

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
        GameCursor = LoadTexture("immagini/grogu.png");
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
        BeginDrawing();
        ClearBackground(Color{ 58, 46, 63, 255 });

        UpdateMusicStream(GameMusic);

        Gamelayer.UpdateGameStatus(GetFrameTime());
        MenuSystem.UpdateSystem();

        AudioManager();

        if (GameStatus != RUNNING) 
            DrawTextureEx(GameCursor, { GetMousePosition().x - 10 * SCALE, GetMousePosition().y }, 0.0f, SCALE * 2, WHITE);

        EndDrawing();
    }

    SettingsManager("SpaceInvadersSettings.json", MenuSystem).SaveData();
    CloseWindow();
}

void Game::AudioManager()
{
    SetMusicVolume(GameMusic,   (MenuSystem.GetMusicVolume() / 100) *      MenuSystem.GetGeneralVolume());

    Esplosione::volume =        (MenuSystem.GetExplosionVolume() / 100) *  MenuSystem.GetGeneralVolume();
    Laser::volume =             (MenuSystem.GetLaserVolume() / 100) *      MenuSystem.GetGeneralVolume();
    Asteroide::volume =         (MenuSystem.GetAsteroidVolume() / 100) *   MenuSystem.GetGeneralVolume();
    PowerUp::volume =           (MenuSystem.GetPowerUpVolume() / 100) *    MenuSystem.GetGeneralVolume();
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
    MenuSystem.CallStart = [this]() { Gamelayer.Start(); };
    MenuSystem.CallRestart = [this]() { Gamelayer.Restart(); };
    MenuSystem.CallResume = [this]() { Gamelayer.Resume(); };
    MenuSystem.CallGetScore = [this]() { return Gamelayer.GetGameScore(); };
    MenuSystem.CallSetDiff = [this]() { Gamelayer.SetDiff(); };
    MenuSystem.InitLayers();
}