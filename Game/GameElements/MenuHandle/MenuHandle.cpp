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
}

void MenuHandle::InitStartMenu()
{
    StartMenu = std::make_shared<Menu>();

    StartMenu->add<Button>("buttonTitle", Strings::title,                                              GameFontSemiBold, 5 * fontSize, 1000, 400, gameContext.renderer.BASE_WIDTH / 2, 5 * 30, nullptr, borderRadius, 10, 4, TEXT_COLOR_NHOVER, TRANSPARENT_COLOR, TRANSPARENT_COLOR, TRANSPARENT_COLOR, TRANSPARENT_COLOR);

    StartMenu->add<TextureButton>("buttonStartMulti", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 - 210), AssetsManager::GetTexture("btnStartMultiPlayer"), [this]() { if (StartMultiPlayer) StartMultiPlayer(); });
    StartMenu->add<TextureButton>("buttonStart", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 - 70), AssetsManager::GetTexture("btnStartSinglePlayer"), [this]() { if (StartSinglePlayer) StartSinglePlayer(); });
    StartMenu->add<Button>("difficButton", TextFormat(Strings::difficulty, TranslateToDifficulty()),    GameFontMedium, fontSize, buttonWidth, buttonHeight, gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 70), [this]() { this->SetDifficulty(); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    StartMenu->add<TextureButton>("txtBtnQuit", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 210), AssetsManager::GetTexture("btnQuitGame"), [this]() { shouldQuit = true; });
}

void MenuHandle::ShowCountDown()
{

    int remainingSeconds = ((int)timerDelayResume.duration.count() - timerDelayResume.elapsedTime()) / 1000 + 1;

    Color c;
    if (remainingSeconds <= 3 && remainingSeconds > 2)
        c = GREEN;
    else if (remainingSeconds <= 2 && remainingSeconds > 1)
        c = YELLOW;
    else
        c = RED;

    const char* text = TextFormat("%d", remainingSeconds);

    float fontSize = 200.0f;
    Vector2 textSize = MeasureTextEx(AssetsManager::GetFontEx("medium", fontSize, nullptr, 0), text, fontSize, 5);
    Vector2 pos = {
        (gameContext.renderer.BASE_WIDTH - textSize.x) / 2.0f,
        (gameContext.renderer.BASE_HEIGHT - textSize.y) / 2.0f
    };

    DrawTextEx(AssetsManager::GetFontEx("medium", fontSize, nullptr, 0), text, {pos.x + 4, pos.y + 4}, fontSize, 5, BLACK);
    DrawTextEx(AssetsManager::GetFontEx("medium", fontSize, nullptr, 0), text, pos, fontSize, 5, c);
}

void MenuHandle::InitRunOverlay()
{
    RunningMenu = std::make_shared<Menu>();

    RunningMenu->add<Label>("labelDanger", Strings::danger, nullptr, GameFontMedium, 70, gameContext.renderer.BASE_WIDTH - MeasureText(Strings::danger, 70) / 2 * 1.5f, MeasureText(Strings::danger, 70) / 4, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    RunningMenu->add<Label>("labelScore", TextFormat(Strings::score, [this]() { if (GetScore) { return GetScore(); }}), nullptr, GameFontMedium, 140, MeasureText(Strings::score, 140) / 2, 140 / 2, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
}

void MenuHandle::InitPausedMenu()
{
    PausedMenu = std::make_shared<Menu>();

    PausedMenu->add<Label>("labelMenuScore", TextFormat(Strings::score, [this]() { if (GetScore) { return GetScore(); }}), nullptr, GameFontMedium, 100, centerX, blockSpacing, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);

    PausedMenu->add<Button>("btnResume", Strings::resume, GameFontMedium,                                     fontSize, 240, buttonHeight, (gameContext.renderer.BASE_WIDTH / 2 - 130), (gameContext.renderer.BASE_HEIGHT / 2 - 200), [this]() { ResumeGame(); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    PausedMenu->add<Button>("btnRestart", Strings::restart, GameFontMedium,                                   fontSize, 240, buttonHeight, (gameContext.renderer.BASE_WIDTH / 2 + 130), (gameContext.renderer.BASE_HEIGHT / 2 - 200), [this]() { if (Restart) Restart(); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);

    PausedMenu->add<Button>("btnctrl", Strings::control, GameFontMedium,                                      fontSize, 240, buttonHeight, (gameContext.renderer.BASE_WIDTH / 2 - 130), (gameContext.renderer.BASE_HEIGHT / 2 - 60), [this]() { MainMenuHandler.PopMenu(); MainMenuHandler.PushMenu(ControlsMenu); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    PausedMenu->add<Button>("btnAudio", Strings::audioSettings, GameFontMedium,                               fontSize, 240, buttonHeight, (gameContext.renderer.BASE_WIDTH / 2 + 130), (gameContext.renderer.BASE_HEIGHT / 2 - 60), [this]() { MainMenuHandler.PopMenu(); MainMenuHandler.PushMenu(AudioMenu); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);

    PausedMenu->add<Button>("difficButton2", TextFormat(Strings::difficulty, TranslateToDifficulty()), GameFontMedium, fontSize, buttonWidth, buttonHeight, gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 80), [this]() { this->SetDifficulty(); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    PausedMenu->add<Button>("btnFullScreen", TextFormat(Strings::fullscreen, fullscreen ? "On" : "Off"), GameFontMedium, fontSize, buttonWidth, buttonHeight, gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 220), [this]() { this->ToggleFullscreen(); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    PausedMenu->add<TextureButton>("btnQuit", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 360), AssetsManager::GetTexture("btnQuit"), [this]() { GoBackToMain(); });
}

void MenuHandle::InitAudioControl()
{
    AudioMenu = std::make_shared<Menu>();

    GeneralVolume = SettingsManager::GetKey<float>("audio", "GeneralVolume");
    MusicVolume = SettingsManager::GetKey<float>("audio", "MusicVolume");
    LaserVolume = SettingsManager::GetKey<float>("audio", "LaserVolume");
    MeteorDamageVolume = SettingsManager::GetKey<float>("audio", "MeteorDamageVolume");
    ExplosionVolume = SettingsManager::GetKey<float>("audio", "ExplosionVolume");
    PowerUpVolume = SettingsManager::GetKey<float>("audio", "PowerUpVolume");

    // BLOCCO 1
    AudioMenu->add<Label>("labelGeneralAudio", Strings::generalVolume, nullptr, GameFontMedium, fontSize, sliderX, centerY - 3 * blockSpacing, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    rect = RectS(sliderX, centerY - 3 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 1, &GeneralVolume, FILL_COLOR_NHOVER);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, FILL_COLOR_HOVER);
    AudioMenu->add<Slider>("GeneralSliderVolume", rect, pointer);

    // BLOCCO 2
    AudioMenu->add<Label>("labelMusicAudio", Strings::musicVolume, nullptr, GameFontMedium, fontSize, sliderX, centerY - 2 * blockSpacing, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    rect = RectS(sliderX, centerY - 2 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &MusicVolume, FILL_COLOR_NHOVER);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, FILL_COLOR_HOVER);
    AudioMenu->add<Slider>("MusicSliderVolume", rect, pointer);

    // BLOCCO 3
    AudioMenu->add<Label>("labelLaserAudio", Strings::laserVolume, nullptr, GameFontMedium, fontSize, sliderX, centerY - 1 * blockSpacing, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    rect = RectS(sliderX, centerY - 1 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &LaserVolume, FILL_COLOR_NHOVER);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, FILL_COLOR_HOVER);
    AudioMenu->add<Slider>("LaserSliderVolume", rect, pointer);

    // BLOCCO 4
    AudioMenu->add<Label>("labelAsteoridAudio", Strings::meteorVolume, nullptr, GameFontMedium, fontSize, sliderX, centerY + 0 * blockSpacing, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    rect = RectS(sliderX, centerY + 0 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &MeteorDamageVolume, FILL_COLOR_NHOVER);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, FILL_COLOR_HOVER);
    AudioMenu->add<Slider>("AsteroidSliderVolume", rect, pointer);

    // BLOCCO 5
    AudioMenu->add<Label>("labelExplosionAudio", Strings::explosionVolume, nullptr, GameFontMedium, fontSize, sliderX, centerY + 1 * blockSpacing, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    rect = RectS(sliderX, centerY + 1 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &ExplosionVolume, FILL_COLOR_NHOVER);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, FILL_COLOR_HOVER);
    AudioMenu->add<Slider>("ExplosionSliderVolume", rect, pointer);

    // BLOCCO 6
    AudioMenu->add<Label>("labelPowerUpsAudio", Strings::modifierVolume, nullptr, GameFontMedium, fontSize, sliderX, centerY + 2 * blockSpacing, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    rect = RectS(sliderX, centerY + 2 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &PowerUpVolume, FILL_COLOR_NHOVER);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, FILL_COLOR_HOVER);
    AudioMenu->add<Slider>("PowerUpSliderVolume", rect, pointer);

    AudioMenu->add<Button>("Go Back Button", "Go Back", GameFontMedium, fontSize, buttonWidth / 2, buttonHeight, centerX, (centerY + buttonWidth), nullptr, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
}

void MenuHandle::InitBindsControls()
{
    ControlsMenu = std::make_shared<Menu>();

    // loading
    gameContext.keyBindings.forEachMutable([this](const std::string& key, int val)
    {
        gameContext.keyBindings.setKey(key, SettingsManager::GetKey<int>("KeyBindings", key));
    });

    // checking and reset
    gameContext.keyBindings.forEach([this](const std::string& key, int val)
    {
        if (gameContext.keyBindings.getKey(key) <= 0)
            gameContext.keyBindings = KeyBindings();
    });

    // constructing buttons
    int i = 0;
    gameContext.keyBindings.forEach([this, &i](const std::string& action, int key)
    {
        std::string btnID = "btn" + action;

        ControlsMenu->add<Button>(
            btnID,
            TextFormat("%s: %s", action.c_str(), TranslateKey(key)),
            GameFontMedium, fontSize, buttonWidth, buttonHeight,
            centerX, yStart + (fontSize + spacing) * i,
            [this, btnID]() { setToBind(btnID); },
            borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER
        );
        i++;
    });

    ControlsMenu->add<Button>("Reset", Strings::reset, GameFontMedium, fontSize, buttonWidth, buttonHeight, centerX, yStart + (fontSize + spacing) * 6, [this]() { setToBind("Reset"); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    ControlsMenu->add<Button>("GoBackButton", "Go Back", GameFontMedium, fontSize, buttonWidth / 2, buttonHeight, centerX, (centerY + buttonWidth), [this]() { MainMenuHandler.PopMenu(); MainMenuHandler.PushMenu(PausedMenu); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
}

void MenuHandle::GoBackToMain()
{
    if (BackToMainMenu)
        BackToMainMenu();
    isSinglePlayer = false;
    *(gameContext.gameStatus) = GameState::ON_START_MENU;
    MainMenuHandler.PushMenu(StartMenu);
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
}
void MenuHandle::SetMenuMultiPlayer()
{
    isSinglePlayer = false;
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
    }
}
void MenuHandle::InitLinksIcons()
{
    instagram = AssetsManager::GetTexture("instagram");
    github = AssetsManager::GetTexture("github");
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

void MenuHandle::RecalculateLayout()
{
    centerY = static_cast<float>(gameContext.renderer.BASE_HEIGHT / 2);
    centerX = static_cast<float>(gameContext.renderer.BASE_WIDTH / 2);
    blockSpacing = 140;
    sliderOffset = 50;
    labelX = static_cast<float>(gameContext.renderer.BASE_WIDTH / 2);
    sliderX = static_cast<float>(gameContext.renderer.BASE_WIDTH / 2);
    sliderWidth = 400;
    sliderHeight = 60;
    pointerWidth = 20;
    borderRadius = 0.4f;
    fontSize = 50;
    buttonHeight = 120;
    buttonWidth = 500;
    spacing = 90;
    totalHeight = (buttonHeight * 6) + (spacing * 5);
    yStart = (gameContext.renderer.BASE_HEIGHT / 2) - (totalHeight / 2);
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