#include "Game.hpp"

#include "../SinglePlayerMode/SinglePlayerMode.hpp"
#include "../MultiPlayerMode/MultiPlayerMode.hpp"

Game::Game() :
    gameStatus(std::make_shared<GameState>(GameState::ON_START_MENU)),
    gameContext{ gameStatus, keyBindings, renderer },
    backgroundStars(gameContext)
{
    gameContext.renderer.Init();

    AssetsManager::Init(PATH_ASSETS_CONFIG);
    SettingsManager::Init(PATH_SPACEINVADER_SETTINGS);
    AudioManager::Init();

    menuHandle = std::make_shared<MenuHandle>(gameContext);
    SetUICallBacks();

    SetExitKey(KEY_NULL);

    InitAudioDevice();
    gameMusic = AssetsManager::GetMusic("game_music");
    gameCursor = AssetsManager::GetTexture("game_cursor");

    HideCursor();
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

        if (*gameStatus != GameState::RUNNING_GAME)
        {
            Vector2 vMouse = renderer.GetVirtualMouse();
            DrawTexture(gameCursor, vMouse.x, vMouse.y, WHITE);
        }

        renderer.EndAndDraw();
    }
    SettingsManager::SaveData(gameContext, *menuHandle);
    AssetsManager::Unload();
    CloseWindow();
}
void Game::Update()
{
    float dt = GetFrameTime();
    UpdateMusicStream(gameMusic);
    backgroundStars.updateStars(dt);

    menuHandle->Update();
    if (currentMode && *gameStatus == GameState::RUNNING_GAME)
        currentMode->Update(dt);

    if (currentMode && *gameStatus == GameState::ON_PAUSED_MENU)
        AudioManager::Update();
}

void Game::LoadSinglePlayerMode()
{
    if (currentMode)
        currentMode->OnExit();

    currentMode = std::make_unique<SinglePlayerMode>(gameContext, *menuHandle);
    currentMode->Init();
    *gameStatus = GameState::RUNNING_GAME;
    menuHandle->SetMenuSinglePlayer();
}
void Game::LoadMultiPlayerMode()
{
    if (currentMode)
        currentMode->OnExit();

    currentMode = std::make_unique<MultiPlayerMode>(gameContext, *menuHandle);
    currentMode->Init();
    *gameStatus = GameState::RUNNING_GAME;
    menuHandle->SetMenuMultiPlayer();
}
void Game::SetUICallBacks()
{
    menuHandle->StartSinglePlayer = [this]() { this->LoadSinglePlayerMode(); };
    menuHandle->StartMultiPlayer = [this]() { this->LoadMultiPlayerMode(); };
    menuHandle->Restart = [this]() { LoadSinglePlayerMode(); };
    menuHandle->BackToMainMenu = [this]() { if (currentMode) { currentMode->OnExit(); currentMode.reset(); } };
}

void Game::DrawBackGround()
{
    ClearBackground(Color{18, 24, 38, 255});
    backgroundStars.DrawStars();
}
