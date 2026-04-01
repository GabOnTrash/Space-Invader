#include "Game.hpp"

#include "../SinglePlayerMode/SinglePlayerMode.hpp"
#include "../MultiPlayerMode/MultiPlayerMode.hpp"

Game::Game() :
    gameStatus(std::make_shared<GameState>(GameState::ON_START_MENU)),
    gameContext{ gameStatus, keyBindings, renderer },
    backgroundStars(gameContext)
{
    InitAudioDevice();
    gameContext.renderer.Init();

    Logger::Get().Init("Server_Log.txt");

    AssetsManager::Init(PATH_ASSETS_CONFIG);
    SettingsManager::Init(PATH_SPACEINVADER_SETTINGS);

    menuHandle = std::make_shared<MenuHandle>(gameContext);
    SetUICallBacks();

    SetExitKey(KEY_NULL);

    gameMusic = AssetsManager::GetMusic("game_music");
    gameCursor = AssetsManager::GetTexture("game_cursor");

    HideCursor();
    PlayMusicStream(gameMusic);
}
Game::~Game()
{
    CloseAudioDevice();
}
void Game::Run()
{
    while (!WindowShouldClose() && !menuHandle->UserWantsToQuit())
    {
        Update();
        renderer.Begin();

        DrawBackGround();

        if (currentMode)
            currentMode->Draw();

        menuHandle->Draw();

        if (*gameStatus != GameState::RUNNING_MULTI_PLAYER
            && *gameStatus != GameState::RUNNING_SINGLE_PLAYER)
        {
            Vector2 vMouse = renderer.GetVirtualMouse();
            DrawTexture(gameCursor, vMouse.x, vMouse.y, WHITE);
        }

        renderer.EndAndDraw();
    }
    AssetsManager::Unload();
    CloseWindow();
}
void Game::Update()
{
    float dt = GetFrameTime();
    UpdateMusicStream(gameMusic);
    SetMusicVolume(gameMusic, AudioManager::Instance().getCalibratedMusicVolume());

    backgroundStars.updateStars(dt);

    menuHandle->Update();
    if (currentMode)
        currentMode->Update(dt);
}

void Game::LoadSinglePlayerMode()
{
    currentMode = std::make_unique<SinglePlayerMode>(gameContext, *menuHandle);
    currentMode->Init();
    *gameStatus = GameState::RUNNING_SINGLE_PLAYER;
}
void Game::LoadMultiPlayerMode()
{
    std::string ip = menuHandle->GetIP();
    std::string port = menuHandle->GetPort();

    if (ip.empty() || port.empty())
    {
        *gameStatus = GameState::ON_CONNECTION_MENU;
        return;
    }

    uint16_t p = std::stoi(port);
    if (p > 65535)
    {
        *gameStatus = GameState::ON_CONNECTION_MENU;
        return;
    }

    currentMode = std::make_unique<MultiPlayerMode>(gameContext, *menuHandle, ip, p);
    currentMode->Init();
    *gameStatus = GameState::RUNNING_MULTI_PLAYER;
}
void Game::SetUICallBacks()
{
    menuHandle->StartSinglePlayer = [this]() { this->LoadSinglePlayerMode(); };
    menuHandle->StartMultiPlayer = [this]() { this->LoadMultiPlayerMode(); };
    menuHandle->Reset = [this]() { currentMode.reset(); };
}

void Game::DrawBackGround()
{
    ClearBackground(Color{18, 24, 38, 255});
    backgroundStars.DrawStars();
}
