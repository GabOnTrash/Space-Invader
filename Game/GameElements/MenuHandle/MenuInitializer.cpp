#include "MenuHandle.hpp"
#include "TextBox.hpp"

void MenuHandle::InitStartMenu()
{
    StartMenu = std::make_shared<Menu>();

    StartMenu->add<Button>("buttonTitle", Strings::title,                                              GameFontSemiBold, 5 * fontSize, 1000, 400, gameContext.renderer.BASE_WIDTH / 2, 5 * 30, nullptr, borderRadius, 10, 4, TEXT_COLOR_NHOVER, TRANSPARENT_COLOR, TRANSPARENT_COLOR, TRANSPARENT_COLOR, TRANSPARENT_COLOR);

    StartMenu->add<TextureButton>("buttonStartMulti", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 - 210), AssetsManager::GetTexture("btnStartMultiPlayer"), [this]() { *(gameContext.gameStatus) = GameState::ON_IP_MENU; });
    StartMenu->add<TextureButton>("buttonStart", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 - 70), AssetsManager::GetTexture("btnStartSinglePlayer"), [this]() { if (StartSinglePlayer) StartSinglePlayer(); });
    StartMenu->add<Button>("difficButton", TextFormat(Strings::difficulty, TranslateToDifficulty()),    GameFontMedium, fontSize, buttonWidth, buttonHeight, gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 70), [this]() { this->SetDifficulty(); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    StartMenu->add<TextureButton>("txtBtnQuit", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 210), AssetsManager::GetTexture("btnQuitGame"), [this]() { shouldQuit = true; });
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

    AudioMenu->add<Button>("Go Back Button", "Go Back", GameFontMedium, fontSize, buttonWidth / 2, buttonHeight, centerX, (centerY + buttonWidth), [this]() { MainMenuHandler.PopMenu(); MainMenuHandler.PushMenu(PausedMenu); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
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

void MenuHandle::InitConnectionMenu()
{
    ConnectionMenu = std::make_shared<Menu>();

    ConnectionMenu->add<TextBox>("txtIp", gameContext.renderer.BASE_WIDTH / 2, gameContext.renderer.BASE_HEIGHT / 3, sliderWidth * 2, sliderHeight * 2, GameFontMedium, fontSize * 1.5, 15, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER);
    ConnectionMenu->add<TextBox>("txtPort", gameContext.renderer.BASE_WIDTH / 2, gameContext.renderer.BASE_HEIGHT / 3 + 130, sliderWidth * 2, sliderHeight * 2, GameFontMedium, fontSize * 1.5, 5, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER);
    ConnectionMenu->add<Label>("lblStatus", "", nullptr, GameFontMedium, 40, gameContext.renderer.BASE_WIDTH / 2, gameContext.renderer.BASE_HEIGHT / 2 + 50, RED, RED);

    ConnectionMenu->add<TextureButton>("btnConnect", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 200), AssetsManager::GetTexture("btnResume"), [this]() { if (StartMultiPlayer) StartMultiPlayer(); });
    ConnectionMenu->add<TextureButton>("btQuit", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 360), AssetsManager::GetTexture("btnQuit"), [this]() { GoBackToMain(); });
}
void MenuHandle::InitMultiPlayerOverlay()
{
    MultiPlayerOverlay = std::make_shared<Menu>();

    MultiPlayerOverlay->add<Label>("lblPlayerCount", "Players: 0", nullptr, GameFontMedium, 30, 150, 50, WHITE, WHITE);
    MultiPlayerOverlay->add<Label>("lblMyID", "ID: ?", nullptr, GameFontMedium, 30, gameContext.renderer.BASE_WIDTH - 150, 50, GREEN, GREEN);
}

void MenuHandle::InitLinksIcons()
{
    instagram = AssetsManager::GetTexture("instagram");
    github = AssetsManager::GetTexture("github");
}