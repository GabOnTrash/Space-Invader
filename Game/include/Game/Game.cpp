#include "Game.hpp"

#include "../SinglePlayerMode/SinglePlayerMode.hpp"

Game::Game() :
    gameStatus(std::make_shared<GameState>(GameState::ON_START_MENU)),
    gameContext{ renderer.BASE_WIDTH, renderer.BASE_HEIGHT, gameStatus, difficulty, keyBindings }
{
    renderer.Init();

    menuHandle = std::make_shared<MenuHandle>(gameContext);

    AssetsManager::Init(PATH_ASSETS_CONFIG);
    SettingsManager::Init(PATH_SPACEINVADER_SETTINGS);
    AudioManager::Init();

    difficulty = static_cast<Difficulty>(SettingsManager::GetKey<int>("difficulty", "GameDifficulty"));

    SetExitKey(KEY_NULL);

    InitAudioDevice();
    gameMusic = AssetsManager::GetMusic("game_music");
    gameCursor = AssetsManager::GetTexture("game_cursor");
}
Game::~Game()
{
    AssetsManager::Unload();
    CloseAudioDevice();
}
void Game::Run()
{
    // modifica dell'handler dei menu, e modifica singole classi per richiesta texture

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
    SettingsManager::SaveData(keyBindings);
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

    currentMode = std::make_unique<SinglePlayerMode>(gameContext);
    currentMode->Init();
    *gameStatus = GameState::RUNNING_GAME;
    menuHandle->SetMenuSinglePlayer();
}

void Game::LoadMultiPlayerMode()
{
    if (currentMode)
        currentMode->OnExit();

    currentMode = std::make_unique<MultiPlayerMode>(gameContext);
    currentMode->Init();
    *gameStatus = GameState::RUNNING_GAME;
    menuHandle->SetMenuMultiPlayer();
}

void Game::SetUICallBacks()
{
    menuHandle->StartSinglePlayer = [this]() { this->LoadSinglePlayerMode(); };
    menuHandle->StartMultiPlayer = [this]() { this->LoadMultiPlayerMode(); };
    menuHandle->Restart = [this]() { if (currentMode) currentMode->Init(); };
    menuHandle->Resume = [this]() { if (currentMode) currentMode->Resume(); };
}

void Game::DrawBackGround()
{
    ClearBackground(Color{18, 24, 38, 255});
    backgroundStars.DrawStars();
}
