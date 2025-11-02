#include "gioco.hpp"

int WINDOW_WIDTH = 1500;
int WINDOW_HEIGHT = 1500;
float SCALE = 1.0f;

Game::Game()
    : tAsteroidi(0, [this]() { asteroidi.emplace_back(); }, true, true)
    , tPotenziamenti(6000, [this]() { CreatePowerUp(); }, true, true)
    , tResume(1500, [this]() { ElementsUpdating = true; }, false, false)
    , PotTriplo(5000, [this]() { giocatore.tripleLaser = false; })
    , PotContinuo(6000, [this]() { giocatore.continued = false; })
    , DpotLento(5000, [this]() { giocatore.reducedVel = false; })
{
    // Window resizing
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Invaders");

    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);

    SCALE = std::min((float) screenWidth / 3840.0f, 1.0f);

    WINDOW_WIDTH = static_cast<int>(WINDOW_WIDTH * SCALE);
    WINDOW_HEIGHT = static_cast<int>(WINDOW_HEIGHT * SCALE);

	SetWindowPosition((screenWidth - WINDOW_WIDTH) / 2, (screenHeight - WINDOW_HEIGHT) / 2);
    SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// end resizing

    asteroidi.reserve(30);
    esplosioni.reserve(10);
    potenziamenti.reserve(10);

	SetExitKey(KEY_NULL);

    InitUI();
    SetDiff();
    LoadAssets();

    collisionThread = std::thread([this]()
        {
            while (runningCollision)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                if (GameStatus == RUNNING && ElementsUpdating)
                {
                    std::lock_guard<std::mutex> lock(collisionMutex);
                    checkAllCollisions();
                }
            }
        });
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

    runningCollision = false;
    if (collisionThread.joinable())
        collisionThread.join();
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
    stelleGioco.InizializzaStelle(0);
    stelleMenu.InizializzaStelle(1);

    PlayMusicStream(GameMusic);

    HideCursor();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{ 58, 46, 63, 255 });

        deltaT = GetFrameTime();

        UpdateMusicStream(GameMusic);

        UpdateGameStatus();
        MenuSystem.UpdateSystem();
        if (GameStatus != RUNNING) DrawTextureEx(GameCursor, { GetMousePosition().x - 10 * SCALE, GetMousePosition().y }, 0.0f, SCALE * 2, WHITE);

        EndDrawing();
    }

    SaveSettingsFile();
    CloseWindow();
}

void Game::CreatePowerUp()
{
    tipoPotenziamento = GetRN<int>(0, DiffPerPotenziamenti);
    potenziamenti.emplace_back(tipoPotenziamento);
}

void Game::UpdateRunInterface()
{
    stelleGioco.AggiornaStelle(deltaT, 0);

    if (ElementsUpdating)
    {
        UpdateTimers();
        UpdateElements();
        giocatore.Aggiorna(deltaT);
    }
    else tResume.update();
}
void Game::DrawRunInterface()
{
    stelleGioco.DisegnaStelle();

    for (auto& asteroide : asteroidi)       asteroide.Disegna();
    for (auto& potenziam : potenziamenti)   potenziam.Disegna();

    giocatore.Disegna();

    for (int i = 0; i < arrayCuori.size(); i++)
    {
        arrayCuori[i].posizione = { static_cast<float>(WINDOW_WIDTH - (i + 1) * arrayCuori[i].getWidth()) - 100 / 10, 0.0f + 100 / 10 };
        arrayCuori[i].Disegna();
    }
}

void Game::UpdateGameStatus()
{
    GameStatus = MenuSystem.GetGameStatus();

    if (GameStatus == START)
    {
        stelleMenu.AggiornaStelle(deltaT, 1);
        stelleMenu.DisegnaStelle();
    }

    if (GameStatus == KILLED)
    {
        MenuSystem.SetKilledGame();

        DrawRunInterface();
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.3f));
    }

    if (GameStatus == RUNNING)
    {
        UpdateRunInterface();
        DrawRunInterface();

        arrayCuori.size() == 0 ? MenuSystem.GetRunningMenu()->activate("labelDanger") : MenuSystem.GetRunningMenu()->deactive("labelDanger");
        !ElementsUpdating ?     MenuSystem.GetRunningMenu()->activate("labelReady") : MenuSystem.GetRunningMenu()->deactive("labelReady");
    }

    if (GameStatus == PAUSED)
    {
        MenuSystem.SetPausedGame();

        DrawRunInterface();
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.3f));
    }

    AudioManager();
}
void Game::ClearEffects()
{
    PotTriplo.deactive();
    PotContinuo.deactive();
    DpotLento.deactive();

    potenziamenti.clear();
    asteroidi.clear();
    esplosioni.clear();
    arrayCuori.clear();

    giocatore.Reset();

    tAsteroidi.duration = std::chrono::milliseconds(400);

    GameScore = 0;
}
void Game::checkAllCollisions()
{
    // Collisioni asteroidi laser
    for (size_t i = 0; i < giocatore.GetLasers().size(); )
    {
        bool laserErased = false;
        for (size_t j = 0; j < asteroidi.size(); )
        {
            if (CheckCollisionRecs(giocatore.GetLasers()[i].getBounds(), asteroidi[j].getBounds()))
            {
                giocatore.GetLasers().erase(giocatore.GetLasers().begin() + i);

                if (asteroidi[j].getState() == DAMAGED)
                {
                    esplosioni.emplace_back(asteroidi[j].getBounds());
                    
                    asteroidi[j] = std::move(asteroidi.back());
                    asteroidi.pop_back();

                    laserErased = true;
                    GameScore++;
                }
                else
                {
                    asteroidi[j].NextState();
                }
                break;
            }
            else
            {
                j++;
            }
        }
        if (!laserErased)
        {
            i++;
        }
    }

    // Collisioni asteroidi bigLaser 
    if (giocatore.continued)
    {
        for (size_t i = 0; i < asteroidi.size(); )
        {
            if (CheckCollisionRecs(giocatore.bigLaser.getBounds(), asteroidi[i].getBounds()))
            {
                if (asteroidi[i].getState() == DAMAGED)
                {
                    esplosioni.emplace_back(asteroidi[i].getBounds());

                    asteroidi[i] = std::move(asteroidi.back());
                    asteroidi.pop_back();

                    GameScore++;
                }
                else
                {
                    asteroidi[i].NextState();
                    i++; // Incrementa qui se non viene cancellato
                }
            }
            else
            {
                i++; 
            }
        }
    }

    // Collisioni asteroidi giocatore
    for (size_t i = 0; i < asteroidi.size(); )
    {
        if (CheckCollisionRecs(giocatore.getBounds(), asteroidi[i].getBounds()))
        {
            if (arrayCuori.size() == 0)
            {
                ClearEffects();
                MenuSystem.SetGameStatus(KILLED);
                MenuSystem.SetLayerPausedMenu();
            }
            else
            {
                arrayCuori.pop_back();
                asteroidi.erase(asteroidi.begin() + i);
            }
        }
        else i++;
    }

    // Collsioni potenziamenti giocatore
    for (size_t i = 0; i < potenziamenti.size(); )
    {
        if (CheckCollisionRecs(giocatore.getBounds(), potenziamenti[i].getBounds()))
        {
            if (potenziamenti[i].tipoPotenziamento == "downgrade")
            {
                giocatore.reducedVel = true;
                DpotLento.active();
            }
            else if (potenziamenti[i].tipoPotenziamento == "depotCuore")
            {
                if (arrayCuori.size() <= 0) MenuSystem.SetGameStatus(KILLED);
                else arrayCuori.pop_back();
            }
            else if (potenziamenti[i].tipoPotenziamento == "triploLaser")
            {
                giocatore.tripleLaser = true;
                PotTriplo.active();
            }
            else if (potenziamenti[i].tipoPotenziamento == "potCuore")
            {
                if (arrayCuori.size() < nMaxCuori) arrayCuori.emplace_back();
            }
            else if (potenziamenti[i].tipoPotenziamento == "bigLaser")
            {
                giocatore.continued = true;
                PotContinuo.active();
            }

            potenziamenti.erase(potenziamenti.begin() + i);
        }
        else i++;
    }
}

void Game::UpdateElements()
{
    std::lock_guard<std::mutex> lock(collisionMutex);

    for (size_t i = 0; i < asteroidi.size(); )
    {
        asteroidi[i].Aggiorna(deltaT);

        if (asteroidi[i].getBounds().y > 100 + WINDOW_HEIGHT)
        {
            asteroidi[i] = std::move(asteroidi.back());
            asteroidi.pop_back();
        }
        else i++;
    }

    for (size_t i = 0; i < potenziamenti.size(); )
    {
        potenziamenti[i].Aggiorna(deltaT);

        if (potenziamenti[i].getBounds().y > 100 + WINDOW_HEIGHT)
        {
            potenziamenti[i] = std::move(potenziamenti.back());
            potenziamenti.pop_back();
        }
        else i++;
    }

    for (size_t i = 0; i < esplosioni.size(); )
    {
        esplosioni[i].Aggiorna(deltaT);

        if (esplosioni[i].fine)
        {
            esplosioni[i] = std::move(esplosioni.back());
            esplosioni.pop_back();
        }
        else i++;
    }
}
void Game::UpdateTimers()
{
    tAsteroidi.update();
    tPotenziamenti.update();

    PotTriplo.update();
    PotContinuo.update();
    DpotLento.update();
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

void Game::Start()
{
    ClearEffects();

    switch (MenuSystem.GetGameDifficulty())
    {
    case EASY:
        arrayCuori.emplace_back();
		[[fallthrough]];
    case NORMAL:
        arrayCuori.emplace_back();
        [[fallthrough]];
    case HARD:
        arrayCuori.emplace_back();
        [[fallthrough]];
    default:
        break;
    }

    MenuSystem.SetLayerGame();
}
void Game::Restart()
{
    Start();
}
void Game::Resume()
{
    esplosioni.clear();
    tResume.deactive();
    tResume.active();

    ElementsUpdating = false;
    MenuSystem.SetLayerGame();
}

void Game::SetDiff()
{
    nMaxCuori = MenuSystem.GetGameDifficulty() - 2;
    tAsteroidi.duration = std::chrono::milliseconds(MenuSystem.GetGameDifficulty() * 100);
    DiffPerPotenziamenti = MenuSystem.GetGameDifficulty() - 1;
}
void Game::InitUI()
{
    MenuSystem.CallStart = [this]() { this->Start(); };
    MenuSystem.CallRestart = [this]() { this->Restart(); };
    MenuSystem.CallResume = [this]() { this->Resume(); };
    MenuSystem.CallGetScore = [this]() { return GameScore; };
    MenuSystem.CallSetDiff = [this]() { this->SetDiff(); };

    MenuSystem.InitLayers();
}

void Game::SaveSettingsFile()
{
    /*SettingsManager sManager("settings.json");
    sManager.SaveMap(VolumeMap, "audio");*/

    SettingsManager settings("SpaceInvadersSettings.json");

    settings.SetKey("audio", "GeneralVolume", MenuSystem.GetGeneralVolume());
    settings.SetKey("audio", "MusicVolume", MenuSystem.GetMusicVolume());
    settings.SetKey("audio", "LaserVolume", MenuSystem.GetLaserVolume());
    settings.SetKey("audio", "AsteroidVolume", MenuSystem.GetAsteroidVolume());
    settings.SetKey("audio", "PowerUpVolume", MenuSystem.GetPowerUpVolume());
    settings.SetKey("audio", "ExplosionVolume", MenuSystem.GetExplosionVolume());

    settings.SetKey("difficulty", "GameDifficulty", MenuSystem.GetGameDifficulty());

    settings.SetKey("KeyBindings", "MOVEUP", KeyBinds.KeyUP);
    settings.SetKey("KeyBindings", "MOVEDOWN", KeyBinds.KeyDOWN);
    settings.SetKey("KeyBindings", "MOVELEFT", KeyBinds.KeyLEFT);
    settings.SetKey("KeyBindings", "MOVERIGHT", KeyBinds.KeyRIGHT);
    settings.SetKey("KeyBindings", "SHOOT", KeyBinds.KeySHOOT);

    settings.SaveData();
}
