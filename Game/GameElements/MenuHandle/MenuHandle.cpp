#include "MenuHandle.hpp"
#include "../Game/Game.hpp"

MenuHandle::MenuHandle(GameContext& ctx)
    : gameContext(ctx)
{
    Widget::setRenderer(&ctx.renderer);

    LoadGameFont(); // maybe will be removed
    settings = std::vector<AudioSetting>{
            {"GeneralVolume",      "labelGeneralAudio",   Strings::generalVolume,  "GeneralSliderVolume",   AudioManager::Instance().getGeneralVolume() },
            {"MusicVolume",        "labelMusicAudio",     Strings::musicVolume,    "MusicSliderVolume",     AudioManager::Instance().getMusicVolume() },
            {"LaserVolume",        "labelLaserAudio",     Strings::laserVolume,    "LaserSliderVolume",     AudioManager::Instance().getLaserVolume() },
            {"MeteorDamageVolume", "labelAsteroidAudio",  Strings::meteorVolume,   "AsteroidSliderVolume",  AudioManager::Instance().getMeteorVolume() },
            {"ExplosionVolume",    "labelExplosionAudio", Strings::explosionVolume, "ExplosionSliderVolume",AudioManager::Instance().getExplosionVolume() },
            {"ModifierVolume",      "labelModifierAudio",  Strings::modifierVolume, "ModifierSliderVolume",   AudioManager::Instance().getModifierVolume() }
    };
    InitAllMenus();

    GameDifficulty = static_cast<Difficulty>(SettingsManager::GetKey<int>("difficulty", "GameDifficulty"));
    SetDifficulty();

    fullscreen = !SettingsManager::GetKey<bool>("video", "Fullscreen");
    ToggleFullscreen();

    menus = {
        { GameState::ON_START_MENU, startMenu },
        { GameState::ON_PAUSE_MENU, pauseMenu },
        { GameState::ON_KILLED_MENU, deathMenu },
        { GameState::ON_AUDIO_MENU, audioMenu },
        { GameState::ON_CONTROLS_MENU, controlsMenu },
        { GameState::ON_CONNECTION_MENU, connectionMenu },
        { GameState::RUNNING_SINGLE_PLAYER, singlePlayerHUD },
        { GameState::RUNNING_MULTI_PLAYER, multiPlayerHUD },
    };

    currentMenu = &menus.at(GameState::ON_START_MENU);
}
void MenuHandle::LoadGameFont()
{
    GameFontMedium = AssetsManager::GetFontEx("medium", 100, nullptr, 0);
    GameFontSemiBold = AssetsManager::GetFontEx("semibold", 100, nullptr, 0);
    SetTextureFilter(GameFontMedium.texture, TEXTURE_FILTER_BILINEAR);
}

void MenuHandle::InitAllMenus()
{
    RecalculateLayout();

    InitStartMenu();
    InitPauseMenu();
    InitAudioMenu();
    InitBindsMenu();
    InitConneMenu();
    InitDeathMenu();

    InitSinglePlayerHUD();
    InitMultiPlayerHUD();
}

void MenuHandle::Update()
{
    currentMenu->Update();

    static GameState lastState = GameState::ON_START_MENU;
    GameState currentState = *(gameContext.gameStatus);

    if (currentState != lastState)
    {
        HandleStateChange(currentState);
        lastState = currentState;
    }

    switch (*(gameContext.gameStatus))
    {
    case GameState::ON_START_MENU:
        if (Reset)
            Reset();
        break;
    case GameState::RUNNING_SINGLE_PLAYER:
        UpdateSinglePlayerHUD();
        if (IsKeyPressed(KEY_ESCAPE))
            *(gameContext.gameStatus) = GameState::ON_PAUSE_MENU;
        break;
    case GameState::RUNNING_MULTI_PLAYER:
        UpdateMultiPlayerHUD();
        if (IsKeyPressed(KEY_ESCAPE))
            *(gameContext.gameStatus) = GameState::ON_PAUSE_MENU;
        break;
    case GameState::ON_CONTROLS_MENU:
        updateKeyBinding();
        break;
    case GameState::ON_PAUSE_MENU:

        break;
    default: ;
    }
}

void MenuHandle::Draw()
{
    switch (*(gameContext.gameStatus))
    {
    case GameState::RUNNING_SINGLE_PLAYER:
        for (auto & i : heartsArray)
            i.Draw();
        break;
    case GameState::RUNNING_MULTI_PLAYER:
    case GameState::ON_CONNECTION_MENU:
        break;
    case GameState::ON_PAUSE_MENU:
    case GameState::ON_AUDIO_MENU:
    case GameState::ON_CONTROLS_MENU:
    case GameState::ON_KILLED_MENU:
        DrawRectangle(0, 0, gameContext.renderer.BASE_WIDTH, gameContext.renderer.BASE_HEIGHT, Fade(BLACK, 0.5f));
        break;
    default: ;
    }

    currentMenu->Draw();
}

void MenuHandle::UpdateSinglePlayerHUD()
{
    for (int i = 0; i < heartsArray.size(); i++)
        heartsArray[i].position = { gameContext.renderer.BASE_WIDTH - (i + 1) * heartsArray[i].getBounds().width - 20, 20 };

    heartsArray.empty() ? singlePlayerHUD.activate("labelDanger") : singlePlayerHUD.deactive("labelDanger");
    singlePlayerHUD.getByID("lblMB")->setText(TextFormat(Strings::mbUsage, GetMemoryUsageMB()));
}

void MenuHandle::UpdateMultiPlayerHUD()
{
    if (!stillTryingConnecting)
    {
        multiPlayerHUD.activate("lblPlayerCount");
        if (lastMatchSize != MATCH_SIZE)
        {
            multiPlayerHUD.getByID("lblPlayerCount")->setText(TextFormat("Players: %d", MATCH_SIZE));
            lastMatchSize = MATCH_SIZE;
        }

        multiPlayerHUD.activate("lblMyID");
        if (lastID != ID)
        {
            multiPlayerHUD.getByID("lblMyID")->setText(TextFormat("ID: %d", ID));
            lastID = ID;
        }

        multiPlayerHUD.deactive("lblConnecting");
    }
    else
    {
        multiPlayerHUD.activate("lblConnecting");
        multiPlayerHUD.deactive("lblPlayerCount");
        multiPlayerHUD.deactive("lblMyID");
    }
}

std::string MenuHandle::GetIP()
{
    std::vector<std::string> substrs = {
        connectionMenu.getByID("txtFirstTerne")->getText(),
        connectionMenu.getByID("txtSecondTerne")->getText(),
        connectionMenu.getByID("txtThirdTerne")->getText(),
        connectionMenu.getByID("txtFourthTerne")->getText()
    };

    std::string ip = "";
    for (int i = 0; i < substrs.size(); i++)
    {
        ip += substrs[i];
        if (i < 3)
            ip += ".";
    }

    return ip;
}

std::string MenuHandle::GetPort()
{
    return connectionMenu.getByID("txtPort")->getText();
}

bool MenuHandle::IsFullscreen()
{
    return fullscreen;
}

void MenuHandle::UpdateDifficulty()
{
    heartsArray.clear();
    nMaxHearts = GameDifficulty - 2;
    DiffPerModifiers = GameDifficulty - 1;

    switch (GameDifficulty)
    {
    case EASY:
        heartsArray.emplace_back();
        [[fallthrough]];
    case NORMAL:
        heartsArray.emplace_back();
        [[fallthrough]];
    case HARD:
        heartsArray.emplace_back();
        [[fallthrough]];
    default:
        break;
    }
}

void MenuHandle::setCurrentScore(int gameScore)
{
    singlePlayerHUD.getByID("labelScore")->setText(TextFormat(Strings::score, gameScore));
    deathMenu.getByID("labelMenuScore")->setText(TextFormat(Strings::score, gameScore));
}

void MenuHandle::RenderCountDown(int remainingSeconds)
{
    Color c;
    if (remainingSeconds <= 3 && remainingSeconds > 2)
        c = GREEN;
    else if (remainingSeconds <= 2 && remainingSeconds > 1)
        c = YELLOW;
    else
        c = RED;

    const char* text = TextFormat("%d", remainingSeconds);

    float fontSize = 200.0f;
    Vector2 textSize = MeasureTextEx(GameFontMedium, text, fontSize, 5);
    Vector2 pos = {
        (gameContext.renderer.BASE_WIDTH - textSize.x) / 2.0f,
        (gameContext.renderer.BASE_HEIGHT - textSize.y) / 2.0f
    };

    DrawTextEx(GameFontMedium, text, {pos.x + 4, pos.y + 4}, fontSize, 5, BLACK);
    DrawTextEx(GameFontMedium, text, pos, fontSize, 5, c);
}

void MenuHandle::ResumeGame(GameState st)
{
    //////// to correct
    shouldResumeTheGame = true;
    //*gameContext.gameStatus = GameState::RUNNING_SINGLE_PLAYER;
    // valid only for singleplayer
}

void MenuHandle::HandleStateChange(GameState newState)
{
    currentMenu = nullptr;
    currentMenu = &menus.at(newState);
}

std::string MenuHandle::SetDifficulty(bool change)
{
    if (change)
        GameDifficulty = static_cast<Difficulty>(3 + ((static_cast<int>(GameDifficulty) - 1) % 3));

    std::string diff = "";
    switch (GameDifficulty)
    {
    case EASY:
        diff = Strings::easy;
        break;
    case NORMAL:
        diff = Strings::normal;
        break;
    case HARD:
        diff = Strings::hard;
        break;
    default:
        diff = Strings::notFound;
    }

    UpdateDifficulty();
    auto p = pauseMenu.getByID("difficButton");
    if (p) p->setText(TextFormat(Strings::difficulty, diff.c_str()));

    p = deathMenu.getByID("difficButton");
    if (p) p->setText(TextFormat(Strings::difficulty, diff.c_str()));
    return diff;
}

void MenuHandle::setToBind(const std::string& id)
{
    if (!waitingForKeyBind.empty())
    {
        std::string oldAction = waitingForKeyBind.substr(3);
        int currentKey = gameContext.keyBindings.getKey(oldAction);

        auto it = RaylibKeyToString.find(currentKey);
        std::string temp = (it != RaylibKeyToString.end()) ? it->second : "Unknown";
        controlsMenu.getByID(waitingForKeyBind)->setText(TextFormat("%s: %s", oldAction.c_str(), temp.c_str()));
    }

    if (id.empty())
    {
        waitingForKeyBind = "";
        return;
    }

    if (id == "Reset")
    {
        gameContext.keyBindings = KeyBindings();
        gameContext.keyBindings.forEach([this](const std::string& key, int val)
        {
            auto it = RaylibKeyToString.find(val);
            std::string temp = (it != RaylibKeyToString.end()) ? it->second : "Unknown";
            controlsMenu.getByID("btn" + key)->setText(TextFormat("%s: %s", key.c_str(), temp.c_str()));
        });

        waitingForKeyBind = "";
        return;
    }

    waitingForKeyBind = id;
    controlsMenu.getByID(id)->setText(Strings::waitingKey);
}

void MenuHandle::updateKeyBinding()
{
    if (waitingForKeyBind.empty()) return;

    int key = GetKeyPressed();
    if (key <= 0) return;

    if (key == KEY_ESCAPE)
    {
        setToBind("");
        return;
    }

    bool alreadyUsed = false;
    if (key == 255 || key > 512) return;
    gameContext.keyBindings.forEach([key, &alreadyUsed](const std::string& k, int val)
    {
        if (val == key) alreadyUsed = true;
    });
    if (alreadyUsed) return;

    std::string actionToBind = waitingForKeyBind.substr(3);
    gameContext.keyBindings.forEachMutable([this, actionToBind, key](const std::string& action, int& val)
    {
        if (actionToBind == action)
        {
            val = key;
            auto it = RaylibKeyToString.find(key);
            std::string temp = (it != RaylibKeyToString.end()) ? it->second : "Unknown";
            controlsMenu.getByID(waitingForKeyBind)->setText(TextFormat("%s: %s", action.c_str(), temp.c_str()));
        }
    });

    waitingForKeyBind = "";
}

void MenuHandle::ToggleFullscreen()
{
    if (fullscreen)
    {
        SetWindowPosition(0, 0);
        SetWindowSize(gameContext.renderer.BASE_WIDTH, gameContext.renderer.BASE_HEIGHT - taskbar);
        pauseMenu.getByID("btnFullScreen")->setText(TextFormat(Strings::fullscreen, "Off"));
        fullscreen = false;
    }
    else
    {
        SetWindowPosition(0, 0);
        SetWindowSize(gameContext.renderer.BASE_WIDTH, gameContext.renderer.BASE_HEIGHT);
        pauseMenu.getByID("btnFullScreen")->setText(TextFormat(Strings::fullscreen, "On"));
        fullscreen = true;
    }
}