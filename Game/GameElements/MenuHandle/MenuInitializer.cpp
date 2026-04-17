#include "MenuHandle.hpp"

void MenuHandle::InitStartMenu()
{
    startMenu.Add<Button>("buttonTitle", Strings::title, GameFontSemiBold, 5 * fontSize, 1000, 400, gameContext.renderer.BASE_WIDTH / 2, 5 * 30, nullptr, borderRadius, 10, 4, TEXT_COLOR_NHOVER, TRANSPARENT_COLOR, TRANSPARENT_COLOR, TRANSPARENT_COLOR, TRANSPARENT_COLOR);

    startMenu.Add<TextureButton>("buttonStartMulti", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 - 210), AssetsManager::GetTexture("btnStartMultiPlayer"), [this]() { *(gameContext.gameStatus) = GameState::ON_CONNECTION_MENU; });
    startMenu.Add<TextureButton>("buttonStart", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 - 70), AssetsManager::GetTexture("btnStartSinglePlayer"), [this]() { if (StartSinglePlayer) StartSinglePlayer(); });

    Texture2D instagram =   AssetsManager::GetTexture("instagram");
    Texture2D github =      AssetsManager::GetTexture("github");

    startMenu.Add<TextureButton>("openIg", gameContext.renderer.BASE_WIDTH - instagram.width * 2, gameContext.renderer.BASE_HEIGHT - instagram.height * 2, instagram, []() { OpenURL("https://www.instagram.com/_gabrielearmenise"); });
    startMenu.Add<TextureButton>("openGh", gameContext.renderer.BASE_WIDTH - github.width * 3.5, gameContext.renderer.BASE_HEIGHT - github.height * 2, github, []() { OpenURL("https://www.github.com/GabOnTrash"); });

    startMenu.Add<TextureButton>("txtBtnQuit", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 70), AssetsManager::GetTexture("btnQuitGame"), [this]() { shouldQuit = true; });
}

void MenuHandle::InitPauseMenu()
{
    pauseMenu.Add<Button>("btnResume", Strings::resume, GameFontMedium,                                     fontSize, 240, buttonHeight, (gameContext.renderer.BASE_WIDTH / 2 - 130), (gameContext.renderer.BASE_HEIGHT / 2 - 200), [this]() { shouldResumeTheGame = true; }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    pauseMenu.Add<Button>("btnRestart", Strings::restart, GameFontMedium,                                   fontSize, 240, buttonHeight, (gameContext.renderer.BASE_WIDTH / 2 + 130), (gameContext.renderer.BASE_HEIGHT / 2 - 200), [this]() { wantToRestartSinglePlayer = true; }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);

    pauseMenu.Add<Button>("btnctrl", Strings::control, GameFontMedium,                                      fontSize, 240, buttonHeight, (gameContext.renderer.BASE_WIDTH / 2 - 130), (gameContext.renderer.BASE_HEIGHT / 2 - 60), [this]() { *gameContext.gameStatus = GameState::ON_CONTROLS_MENU; }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    pauseMenu.Add<Button>("btnAudio", Strings::audioSettings, GameFontMedium,                               fontSize, 240, buttonHeight, (gameContext.renderer.BASE_WIDTH / 2 + 130), (gameContext.renderer.BASE_HEIGHT / 2 - 60), [this]() { *gameContext.gameStatus = GameState::ON_AUDIO_MENU; }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);

    pauseMenu.Add<Button>("difficButton", TextFormat(Strings::difficulty, SetDifficulty().c_str()), GameFontMedium, fontSize, buttonWidth, buttonHeight, gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 80), [this]() { this->SetDifficulty(true); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    pauseMenu.Add<Button>("btnFullScreen", TextFormat(Strings::fullscreen, fullscreen ? "On" : "Off"), GameFontMedium, fontSize, buttonWidth, buttonHeight, gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 220), [this]() { this->ToggleFullscreen(); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    pauseMenu.Add<TextureButton>("btnQuit", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 360), AssetsManager::GetTexture("btnQuit"), [this]() { *(gameContext.gameStatus) = GameState::ON_START_MENU; });
}

void MenuHandle::InitDeathMenu()
{
    deathMenu.Add<Label<void>>("labelMenuScore", TextFormat(Strings::score, 0), nullptr, GameFontMedium, 100, centerX, blockSpacing, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    deathMenu.Add<TextureButton>("btnRestart", centerX, (gameContext.renderer.BASE_HEIGHT / 2 - 140), AssetsManager::GetTexture("btnRestart"), [this]() { wantToRestartSinglePlayer = true; });
    deathMenu.Add<Button>("difficButton", TextFormat(Strings::difficulty, SetDifficulty().c_str()), GameFontMedium, fontSize, buttonWidth, buttonHeight, gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2), [this]() { this->SetDifficulty(true); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    deathMenu.Add<TextureButton>("btnQuit", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 140), AssetsManager::GetTexture("btnQuit"), [this]() { *(gameContext.gameStatus) = GameState::ON_START_MENU; });
}

void MenuHandle::InitAudioMenu()
{
    for (size_t i = 0; i < settings.size(); ++i)
    {
        auto& s = settings[i];
        int offsetMultiplier = static_cast<int>(i) - 3;
        float currentY = centerY + (offsetMultiplier * blockSpacing);

        // Label
        audioMenu.Add<Label<void>>(s.labelId, s.labelText, nullptr, GameFontMedium, fontSize, sliderX, currentY, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);

        // Slider
        rect = RectS(sliderX, currentY + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 1, s.volumeVar, FILL_COLOR_NHOVER);
        pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, FILL_COLOR_HOVER);
        audioMenu.Add<Slider>(s.sliderId, rect, pointer);
    }

    audioMenu.Add<Button>("Go Back Button", "Go Back", GameFontMedium, fontSize, buttonWidth / 2, buttonHeight, centerX, (centerY + buttonWidth), [this]() { *(gameContext.gameStatus) = GameState::ON_PAUSE_MENU; }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
}

void MenuHandle::InitBindsMenu()
{
    gameContext.keyBindings.forEachMutable([this](const std::string& key, int val)
    {
        gameContext.keyBindings.setKey(key, SettingsManager::GetKey<int>("KeyBindings", key));
    });

    // checking and reset if error
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

        auto it = RaylibKeyToString.find(key);
        std::string temp = (it != RaylibKeyToString.end()) ? it->second : "Unknown";
        controlsMenu.Add<Button>(
            btnID,
            TextFormat("%s: %s", action.c_str(), temp.c_str()),
            GameFontMedium, fontSize, buttonWidth, buttonHeight,
            centerX, yStart + (fontSize + spacing) * i,
            [this, btnID]() { setToBind(btnID); },
            borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER
        );
        i++;
    });

    controlsMenu.Add<Button>("Reset", Strings::reset, GameFontMedium, fontSize, buttonWidth, buttonHeight, centerX, yStart + (fontSize + spacing) * 6, [this]() { setToBind("Reset"); }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    controlsMenu.Add<Button>("GoBackButton", "Go Back", GameFontMedium, fontSize, buttonWidth / 2, buttonHeight, centerX, (centerY + buttonWidth), [this]() { *(gameContext.gameStatus) = GameState::ON_PAUSE_MENU; }, borderRadius, 0, 4, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER, FILL_COLOR_NHOVER, TEXT_COLOR_NHOVER);
}

void MenuHandle::InitConneMenu()
{
    connectionMenu.Add<Label<void>>("labelIp", "IP:", nullptr, GameFontMedium, 140, gameContext.renderer.BASE_WIDTH / 2 - sliderWidth * 1.6, gameContext.renderer.BASE_HEIGHT / 3, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    connectionMenu.Add<TextBox>("txtFirstTerne", gameContext.renderer.BASE_WIDTH / 2 - sliderWidth + sliderWidth / 4, gameContext.renderer.BASE_HEIGHT / 3, sliderWidth / 2 - 5, sliderHeight * 2, GameFontMedium, fontSize * 1.5, 3, 48, 57, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER);
    connectionMenu.Add<TextBox>("txtSecondTerne", gameContext.renderer.BASE_WIDTH / 2 - sliderWidth / 2 + sliderWidth / 4, gameContext.renderer.BASE_HEIGHT / 3, sliderWidth / 2 - 5, sliderHeight * 2, GameFontMedium, fontSize * 1.5, 3, 48, 57, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER);
    connectionMenu.Add<TextBox>("txtThirdTerne", gameContext.renderer.BASE_WIDTH / 2 + sliderWidth / 4, gameContext.renderer.BASE_HEIGHT / 3, sliderWidth / 2 - 5, sliderHeight * 2, GameFontMedium, fontSize * 1.5, 3, 48, 57, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER);
    connectionMenu.Add<TextBox>("txtFourthTerne", gameContext.renderer.BASE_WIDTH / 2 + sliderWidth / 2 + sliderWidth / 4, gameContext.renderer.BASE_HEIGHT / 3, sliderWidth / 2 - 5, sliderHeight * 2, GameFontMedium, fontSize * 1.5, 3, 48, 57, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER);

    connectionMenu.Add<Label<void>>("labelPort", "Port:", nullptr, GameFontMedium, 140, gameContext.renderer.BASE_WIDTH / 2 - sliderWidth * 1.4, gameContext.renderer.BASE_HEIGHT / 3 + 130, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    connectionMenu.Add<TextBox>("txtPort", gameContext.renderer.BASE_WIDTH / 2, gameContext.renderer.BASE_HEIGHT / 3 + 130, sliderWidth * 2, sliderHeight * 2, GameFontMedium, fontSize * 1.5, 5, 48, 57, TEXT_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_NHOVER, FILL_COLOR_HOVER);

    connectionMenu.Add<TextureButton>("btnConnect", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 200), AssetsManager::GetTexture("btnResume"), [this]() { if (StartMultiPlayer) StartMultiPlayer(); });
    connectionMenu.Add<TextureButton>("btQuit", gameContext.renderer.BASE_WIDTH / 2, (gameContext.renderer.BASE_HEIGHT / 2 + 360), AssetsManager::GetTexture("btnQuit"), [this]() { *(gameContext.gameStatus) = GameState::ON_START_MENU; });
}

void MenuHandle::InitSinglePlayerHUD()
{
    singlePlayerHUD.Add<Label<void>>("labelDanger", Strings::danger, nullptr, GameFontMedium, 70, gameContext.renderer.BASE_WIDTH - MeasureText(Strings::danger, 70) / 2 * 1.5f, MeasureText(Strings::danger, 70) / 4, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    singlePlayerHUD.Add<Label<void>>("labelScore", TextFormat(Strings::score, 0), nullptr, GameFontMedium, 140, MeasureText(Strings::score, 140) / 2, 140 / 2, TEXT_COLOR_NHOVER, TEXT_COLOR_NHOVER);
    singlePlayerHUD.Add<Label<void>>("lblMB", TextFormat(Strings::mbUsage, 0), nullptr, GameFontMedium, fontSize, centerX, 40, GRAY, GRAY);
}

void MenuHandle::InitMultiPlayerHUD()
{
    multiPlayerHUD.Add<Label<void>>("lblConnecting", "Trying to connect...", nullptr, GameFontMedium, fontSize * 3, gameContext.renderer.BASE_WIDTH / 2, gameContext.renderer.BASE_HEIGHT / 2, YELLOW, YELLOW);
    multiPlayerHUD.Add<Label<void>>("lblPlayerCount", "Players: 0", nullptr, GameFontMedium, fontSize, 150, 50, WHITE, WHITE);
    multiPlayerHUD.Add<Label<void>>("lblMyID", "ID: ?", nullptr, GameFontMedium, fontSize, gameContext.renderer.BASE_WIDTH / 2, 150, GREEN, GREEN);
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