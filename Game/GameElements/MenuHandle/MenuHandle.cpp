#include "MenuHandle.hpp"
#include "../Game/Game.hpp"

MenuHandle::MenuHandle(GameContext& ctx)
    : gameContext(ctx), timerDelayResume(3000, [this]() { GameShouldUpdate = true; })
{
    Widget::setRenderer(&ctx.renderer);

    LoadGameFont(); // maybe will be removed
    RecalculateLayout();

    InitAllMenus();
    InitLinksIcons();

    GameDifficulty = static_cast<Difficulty>(SettingsManager::GetKey<int>("difficulty", "GameDifficulty"));
    ChangeDifficulty();

    fullscreen = !SettingsManager::GetKey<bool>("video", "Fullscreen");
    ToggleFullscreen();

    MainMenuHandler.PushMenu(StartMenu);
}

void MenuHandle::Update()
{
    MainMenuHandler.Update();

    if (isSinglePlayer)
        timerDelayResume.update();

    if (IsKeyPressed(KEY_ESCAPE) && *(gameContext.gameStatus) != GameState::KILLED)
        *(gameContext.gameStatus) = GameState::ON_PAUSED_MENU;

    static GameState lastState = GameState::ON_START_MENU;
    GameState currentState = *(gameContext.gameStatus);

    if (currentState != lastState)
    {
        HandleStateChange(currentState);
        lastState = currentState;
    }

    if (!waitingForKeyBind.empty())
        updateKeyBinding();

    if (currentState == GameState::RUNNING_GAME)
    {
        if (isSinglePlayer) UpdateSinglePlayerOverlay();
        else UpdateMultiPlayerOverlay();
    }
}

void MenuHandle::LoadGameFont()
{
    GameFontMedium = AssetsManager::GetFontEx("medium", 100, nullptr, 0);
    GameFontSemiBold = AssetsManager::GetFontEx("semibold", 100, nullptr, 0);
    SetTextureFilter(GameFontMedium.texture, TEXTURE_FILTER_BILINEAR);
}

void MenuHandle::InitAllMenus()
{
    InitStartMenu();
    InitRunOverlay();
    InitPausedMenu();
    InitAudioControl();
    InitBindsControls();
    InitConnectionMenu();
}

void MenuHandle::ShowCountDown()
{
    int remainingSeconds = ((int) timerDelayResume.duration.count() - timerDelayResume.elapsedTime()) / 1000 + 1;

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

void MenuHandle::GoBackToMain()
{
    isSinglePlayer = false;
    *(gameContext.gameStatus) = GameState::ON_START_MENU;
    MainMenuHandler.PushMenu(StartMenu);

    if (BackToMainMenu)
        BackToMainMenu();
}

void MenuHandle::UpdateSinglePlayerOverlay()
{
    *(gameContext.gameStatus) = GameState::RUNNING_GAME;

    for (int i = 0; i < heartsArray.size(); i++)
        heartsArray[i].position = { gameContext.renderer.BASE_WIDTH - (i + 1) * heartsArray[i].getBounds().width - 20, 20 };

    if (heartsArray.empty())
        RunningMenu->activate("labelDanger");

    else
        RunningMenu->deactive("labelDanger");
}

void MenuHandle::UpdateMultiPlayerOverlay()
{

}

void MenuHandle::Draw()
{
    if (*(gameContext.gameStatus) == GameState::ON_START_MENU)
        DrawLinksIcons();

    else if (*(gameContext.gameStatus) != GameState::RUNNING_GAME)
        DrawRectangle(0, 0, gameContext.renderer.BASE_WIDTH, gameContext.renderer.BASE_HEIGHT, Fade(BLACK, 0.5f));

    MainMenuHandler.Draw();

    if (timerDelayResume.isRunning && *(gameContext.gameStatus) == GameState::RUNNING_GAME)
        ShowCountDown();

    if (isSinglePlayer)
        for (auto & i : heartsArray)
            i.Draw();
}

bool MenuHandle::IsFullscreen()
{
    return fullscreen;
}

void MenuHandle::UpdateDifficulty()
{
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

    nMaxHearts = GameDifficulty - 2;
    DiffPerModifiers = GameDifficulty - 1;
}
void MenuHandle::SetMenuSinglePlayer()
{
    isSinglePlayer = true;
    RunningMenu->activate("labelDanger");
    RunningMenu->activate("labelScore");
}
void MenuHandle::SetMenuMultiPlayer()
{
    isSinglePlayer = false;
    RunningMenu->deactive("labelDanger");
    RunningMenu->deactive("labelScore");
    *(gameContext.gameStatus) = GameState::RUNNING_GAME;
}

void MenuHandle::ConnectToServer()
{
    IP = ConnectionMenu->getByID("txtIp")->getText();
    PORT = std::stoi(ConnectionMenu->getByID("txtPort")->getText());

    if (StartMultiPlayer)
        StartMultiPlayer();
}

void MenuHandle::ResumeGame()
{
    *(gameContext.gameStatus) = GameState::RUNNING_GAME;
    GameShouldUpdate = false;
    timerDelayResume.active();
    MainMenuHandler.PopMenu();
}
void MenuHandle::HandleStateChange(GameState newState)
{
    MainMenuHandler.PopMenu();

    switch (newState)
    {
    case GameState::ON_START_MENU:
        MainMenuHandler.PushMenu(StartMenu);
        break;
    case GameState::RUNNING_GAME:
        MainMenuHandler.PushMenu(RunningMenu);
        break;
    case GameState::ON_PAUSED_MENU:
        PausedMenu->activate("btnResume");
        PausedMenu->getByID("btnRestart")->setWidth(240);
        PausedMenu->getByID("btnRestart")->setPosX(centerX + (130));
        MainMenuHandler.PushMenu(PausedMenu);
        break;
    case GameState::KILLED:
        PausedMenu->deactive("btnResume");
        PausedMenu->getByID("btnRestart")->setWidth(buttonWidth);
        PausedMenu->getByID("btnRestart")->setPosX(centerX);
        MainMenuHandler.PushMenu(PausedMenu);
        break;
    case GameState::ON_IP_MENU:
        MainMenuHandler.PushMenu(ConnectionMenu);
        isSinglePlayer = false;
        break;
    }
}

void MenuHandle::DrawLinksIcons()
{
    Vector2 mousePos = gameContext.renderer.GetVirtualMouse();
    
    DrawTexture(instagram, gameContext.renderer.BASE_WIDTH - instagram.width * 2, gameContext.renderer.BASE_HEIGHT - instagram.height * 2, WHITE);
    DrawTexture(github, gameContext.renderer.BASE_WIDTH - github.width * 3.5, gameContext.renderer.BASE_HEIGHT - github.height * 2, WHITE);

    if (CheckCollisionPointRec(mousePos,
        Rectangle{static_cast<float>(gameContext.renderer.BASE_WIDTH - instagram.width * 2),
            static_cast<float>(gameContext.renderer.BASE_HEIGHT - instagram.height * 2),
            static_cast<float>(instagram.width), static_cast<float>(instagram.height) }) && IsMouseButtonPressed(0))
    {
        OpenURL("https://www.instagram.com/_gabrielearmenise");
    }

    if (CheckCollisionPointRec(mousePos,
        Rectangle{static_cast<float>(gameContext.renderer.BASE_WIDTH - github.width * 3.5),
            static_cast<float>(gameContext.renderer.BASE_HEIGHT - github.height * 2.5),
            static_cast<float>(github.width), static_cast<float>(github.height) }) && IsMouseButtonPressed(0))
    {
        OpenURL("https://www.github.com/GabOnTrash");
    }
}

const char* MenuHandle::TranslateToDifficulty() const
{
    switch (GameDifficulty)
    {
    case EASY: return Strings::easy;
    case NORMAL: return Strings::normal;
    case HARD: return Strings::hard;
    default: return Strings::notFound;
    }
}

void MenuHandle::SetDifficulty()
{
    GameDifficulty = static_cast<Difficulty>(3 + ((static_cast<int>(GameDifficulty) - 1) % 3));
    ChangeDifficulty();
}
void MenuHandle::ChangeDifficulty()
{
    StartMenu->getByID("difficButton")->setText(TextFormat(Strings::difficulty, TranslateToDifficulty()));
    PausedMenu->getByID("difficButton2")->setText(TextFormat(Strings::difficulty, TranslateToDifficulty()));
}

void MenuHandle::setToBind(const std::string& id)
{
    if (!waitingForKeyBind.empty())
    {
        std::string oldAction = waitingForKeyBind.substr(3);
        int currentKey = gameContext.keyBindings.getKey(oldAction);
        ControlsMenu->getByID(waitingForKeyBind)->setText(TextFormat("%s: %s", oldAction.c_str(), TranslateKey(currentKey)));
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
            ControlsMenu->getByID("btn" + key)->setText(TextFormat("%s: %s", key.c_str(), TranslateKey(val)));
        });

        waitingForKeyBind = "";
        return;
    }

    waitingForKeyBind = id;
    auto btnToBind = ControlsMenu->getByID(id);
    if (btnToBind)
        btnToBind->setText(Strings::waitingKey);
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
    if (key == 256 || key > 512) return;
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
            ControlsMenu->getByID(waitingForKeyBind)
            ->setText(TextFormat("%s: %s", action.c_str(), TranslateKey(key)));
        }
    });

    waitingForKeyBind = "";
}
const char* MenuHandle::TranslateKey(int key)
{
    static std::string temp;

    auto it = RaylibKeyToString.find(key);
    temp = (it != RaylibKeyToString.end()) ? it->second : "Unknown";
    
    return temp.c_str();
}

void MenuHandle::ToggleFullscreen()
{
    if (fullscreen)
    {
        SetWindowPosition(0, 0);
        SetWindowSize(gameContext.renderer.BASE_WIDTH, gameContext.renderer.BASE_HEIGHT - taskbar);
        PausedMenu->getByID("btnFullScreen")->setText(TextFormat(Strings::fullscreen, "Off"));
        fullscreen = false;
    }
    else
    {
        SetWindowPosition(0, 0);
        SetWindowSize(gameContext.renderer.BASE_WIDTH, gameContext.renderer.BASE_HEIGHT);
        PausedMenu->getByID("btnFullScreen")->setText(TextFormat(Strings::fullscreen, "On"));
        fullscreen = true;
    }
}