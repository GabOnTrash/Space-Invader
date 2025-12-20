#include "interface.hpp"
#include "game.hpp"

KeyBindings KeyBinds;

Interface::Interface(std::shared_ptr<GameState> GameStatus) : GameStatus(GameStatus)
{
}
void Interface::InitLayers()
{
    StartMenu = std::make_shared<Menu>();
    RunningMenu = std::make_shared<Menu>();
    PausedMenu = std::make_shared<Menu>();
    AudioMenu = std::make_shared<Menu>();
    ControlsMenu = std::make_shared<Menu>();

    if (loadedResources)
    {
        GameFont = AssetsManager::GetFontEx("main", 100, nullptr, 0);
        SetTextureFilter(GameFont.texture, TEXTURE_FILTER_BILINEAR);
    }
    RecalculateLayout();

    InitStartMenuSettings();
    InitRunningOverlay();
    InitPausedMenuSettings();
    InitAudioControlSettings();
    InitControlsSettings();

    JsonParser settings(PATH_SPACEINVADERS_SETTINGS);

    fullscreen = !settings.GetKey<bool>("video", "Fullscreen"); // Inverted on purpose to match what we get from the json example (we get true, we set
                                                                // to false, and the toggle set it back to true)
    ToggleFullscreen();

    GameDifficulty = static_cast<Difficulty>(settings.GetKey<int>("difficulty", "GameDifficulty"));

    if (GameDifficulty < 3 || GameDifficulty > 5)
    {
        GameDifficulty = NORMAL;
    }
    ChangeDifficulty();

    MainMenuHandler.PushMenu(StartMenu);
}
void Interface::RecalculateLayout()
{
    centerY = BASE_HEIGHT / 2;
    blockSpacing = 140;
    sliderOffset = 50;
    labelX = BASE_WIDTH / 2;
    sliderX = BASE_WIDTH / 2;
    sliderWidth = 400;
    sliderHeight = 60;
    pointerWidth = 20;
    borderRadius = 0.4f;
    fontSize = 50;
    buttonHeight = 120;
    buttonWidth = 500;
    spacing = 90;
    totalHeight = (buttonHeight * 6) + (spacing * 5);
    yStart = (BASE_HEIGHT / 2) - (totalHeight / 2);
    centerX = BASE_WIDTH / 2;
}

const char* Interface::TranslateToDifficulty()
{
    switch (GameDifficulty)
    {
    case EASY: return Strings::easy;
    case NORMAL: return Strings::normal;
    case HARD: return Strings::hard;
    default: return Strings::notFound;
    }
}

void Interface::SetDifficulty()
{
    GameDifficulty = static_cast<Difficulty>(3 + ((static_cast<int>(GameDifficulty) - 1) % 3));

    if (CallSetDiff) CallSetDiff();

    ChangeDifficulty();
}
void Interface::ChangeDifficulty()
{
    StartMenu->getByID("difficButton")->setText(TextFormat(Strings::difficulty, TranslateToDifficulty()));
    PausedMenu->getByID("difficButton2")->setText(TextFormat(Strings::difficulty, TranslateToDifficulty()));
}

void Interface::setToBind(const std::string& id)
{
    if (id == "Reset")
    {
        KeyBinds.KeyUP = KEY_W;
        KeyBinds.KeyLEFT = KEY_A;
        KeyBinds.KeyDOWN = KEY_S;
        KeyBinds.KeyRIGHT = KEY_D;
        KeyBinds.KeySHOOT = KEY_SPACE;

        ControlsMenu->getByID("btnMoveUp")->setText(TextFormat(Strings::moveup, TranslateKey(KeyBinds.KeyUP)));
        ControlsMenu->getByID("btnMoveDown")->setText(TextFormat(Strings::movedown, TranslateKey(KeyBinds.KeyDOWN)));
        ControlsMenu->getByID("btnMoveLeft")->setText(TextFormat(Strings::moveleft, TranslateKey(KeyBinds.KeyLEFT)));
        ControlsMenu->getByID("btnMoveRight")->setText(TextFormat(Strings::moveright, TranslateKey(KeyBinds.KeyRIGHT)));
        ControlsMenu->getByID("btnShoot")->setText(TextFormat(Strings::shoot, TranslateKey(KeyBinds.KeySHOOT)));
        waitingForKeyBind = "";
        return;
    }

    waitingForKeyBind = id;
    ControlsMenu->getByID(id)->setText(Strings::waitingKey);
}
void Interface::updateKeyBinding()
{
    if (waitingForKeyBind.empty()) return;

    int key = GetKeyPressed();
    if (key <= 0                || 
        key > 512               || 
        key == 256              ||
        key == KeyBinds.KeyUP   || 
        key == KeyBinds.KeyDOWN ||
        key == KeyBinds.KeyLEFT || 
        key == KeyBinds.KeyRIGHT||
        key == KeyBinds.KeySHOOT) return;

    if (waitingForKeyBind == "btnMoveUp") 
    {
        KeyBinds.KeyUP = key;
        ControlsMenu->getByID("btnMoveUp")->setText(TextFormat(Strings::moveup, TranslateKey(key)));
    }
    else if (waitingForKeyBind == "btnMoveDown") 
    {
        KeyBinds.KeyDOWN = key;
        ControlsMenu->getByID("btnMoveDown")->setText(TextFormat(Strings::movedown, TranslateKey(key)));
    }
    else if (waitingForKeyBind == "btnMoveLeft") 
    {
        KeyBinds.KeyLEFT = key;
        ControlsMenu->getByID("btnMoveLeft")->setText(TextFormat(Strings::moveleft, TranslateKey(key)));
    }
    else if (waitingForKeyBind == "btnMoveRight")
    {
        KeyBinds.KeyRIGHT = key;
        ControlsMenu->getByID("btnMoveRight")->setText(TextFormat(Strings::moveright, TranslateKey(key)));
    }
    else if (waitingForKeyBind == "btnShoot") 
    {
        KeyBinds.KeySHOOT = key;
        ControlsMenu->getByID("btnShoot")->setText(TextFormat(Strings::shoot, TranslateKey(key)));
    }

    waitingForKeyBind.clear();
}
const char* Interface::TranslateKey(int key)
{
    static std::string temp;

    auto it = RaylibKeyToString.find(key);
    temp = (it != RaylibKeyToString.end()) ? it->second : "Unknown";
    
    return temp.c_str();
}

void Interface::InitStartMenuSettings()
{
    StartMenu->add<Button>("bottoneStart", Strings::title,                                              GameFont, 2 * fontSize, 1000, 400, BASE_WIDTH / 2, 100, nullptr, borderRadius, 10, 4, GrigioChiaro, transparent, transparent, transparent, transparent);
    StartMenu->add<Button>("bottoneStart", Strings::start,                                              GameFont, fontSize, buttonWidth, buttonHeight, BASE_WIDTH / 2, (BASE_HEIGHT / 2 - 140), [this]() { if (CallRestart) this->CallStart(); }, borderRadius, 10, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    StartMenu->add<Button>("difficButton", TextFormat(Strings::difficulty, TranslateToDifficulty()),    GameFont, fontSize, buttonWidth, buttonHeight, BASE_WIDTH / 2, (BASE_HEIGHT / 2), [this]() { this->SetDifficulty(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    StartMenu->add<Button>("bottoneQuitFromStart", Strings::quitGame,                                   GameFont, fontSize, buttonWidth, buttonHeight, BASE_WIDTH / 2, (BASE_HEIGHT / 2 + 140), [this]() { shouldQuit = true; }, borderRadius, 10, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
}
void Interface::InitRunningOverlay()
{
    RunningMenu->add<Label>("labelReady", Strings::ready, nullptr, GameFont, 70, BASE_WIDTH / 2, BASE_HEIGHT / 2, WHITE, WHITE);
    RunningMenu->add<Label>("labelScore", Strings::score, [this]() { if (CallGetScore) { return this->CallGetScore(); }; }, GameFont, 60, MeasureText(Strings::score, 60) / 2, 30, WHITE, WHITE);
    RunningMenu->add<Label>("labelDanger", Strings::danger, nullptr, GameFont, 70, BASE_WIDTH - offsetY * 1.5f, offsetY / 2, WHITE, WHITE);
}
void Interface::InitPausedMenuSettings()
{
    PausedMenu->add<Button>("btnResume", Strings::resume, GameFont,                                     fontSize, 240, buttonHeight, (BASE_WIDTH / 2 - 130), (BASE_HEIGHT / 2 - 200), [this]() { if (CallResume) this->CallResume(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    PausedMenu->add<Button>("btnRestart", Strings::restart, GameFont,                                   fontSize, 240, buttonHeight, (BASE_WIDTH / 2 + 130), (BASE_HEIGHT / 2 - 200), [this]() { if (CallRestart) this->CallRestart(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);

    PausedMenu->add<Button>("btnctrl", Strings::control, GameFont,                                      fontSize, 240, buttonHeight, (BASE_WIDTH / 2 - 130), (BASE_HEIGHT / 2 - 60), [this]() { this->SetLayerControls(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    PausedMenu->add<Button>("btnAudio", Strings::audioSettings, GameFont,                               fontSize, 240, buttonHeight, (BASE_WIDTH / 2 + 130), (BASE_HEIGHT / 2 - 60), [this]() { this->SetLayerAudio(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);

    PausedMenu->add<Button>("difficButton2", TextFormat(Strings::difficulty, TranslateToDifficulty()), GameFont, fontSize, buttonWidth, buttonHeight, BASE_WIDTH / 2, (BASE_HEIGHT / 2 + 80), [this]() { this->SetDifficulty(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    PausedMenu->add<Button>("btnFullScreen", TextFormat(Strings::fullscreen, fullscreen ? "On" : "Off"), GameFont, fontSize, buttonWidth, buttonHeight, BASE_WIDTH / 2, (BASE_HEIGHT / 2 + 220), [this]() { this->ToggleFullscreen(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    PausedMenu->add<Button>("btnQuit", Strings::quit, GameFont,                                         fontSize, buttonWidth, buttonHeight, BASE_WIDTH / 2, (BASE_HEIGHT / 2 + 360), [this]() { this->SetLayerStart(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
}
void Interface::InitAudioControlSettings()
{
    JsonParser settings(PATH_SPACEINVADERS_SETTINGS);

    GeneralVolume = settings.GetKey<float>("audio", "GeneralVolume");
    MusicVolume = settings.GetKey<float>("audio", "MusicVolume");
    LaserVolume = settings.GetKey<float>("audio", "LaserVolume");
    MeteorDamageVolume = settings.GetKey<float>("audio", "MeteorDamageVolume");
    ExplosionVolume = settings.GetKey<float>("audio", "ExplosionVolume");
    PowerUpVolume = settings.GetKey<float>("audio", "PowerUpVolume");

    // BLOCCO 1
    AudioMenu->add<Label>("labelGeneralAudio", Strings::GENVOLUME, nullptr, GameFont, 60, labelX, centerY - 3 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY - 3 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 1, &GeneralVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, GrigioScuro);
    AudioMenu->add<Slider>("GeneralSliderVolume", rect, pointer);

    // BLOCCO 2
    AudioMenu->add<Label>("labelMusicAudio", Strings::MUSCVOLUME, nullptr, GameFont, 58, labelX + 7, centerY - 2 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY - 2 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &MusicVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, GrigioScuro);
    AudioMenu->add<Slider>("MusicSliderVolume", rect, pointer);

    // BLOCCO 3
    AudioMenu->add<Label>("labelLaserAudio", Strings::LASVOLUME, nullptr, GameFont, 67, labelX + 8, centerY - 1 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY - 1 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &LaserVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, GrigioScuro);
    AudioMenu->add<Slider>("LaserSliderVolume", rect, pointer);

    // BLOCCO 4
    AudioMenu->add<Label>("labelAsteoridAudio", Strings::ASTVOLUME, nullptr, GameFont, 54, labelX + 4, centerY + 0 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY + 0 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &MeteorDamageVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, GrigioScuro);
    AudioMenu->add<Slider>("AsteroidSliderVolume", rect, pointer);

    // BLOCCO 5
    AudioMenu->add<Label>("labelExplosionAudio", Strings::EXPLVOLUME, nullptr, GameFont, 51, labelX - 2, centerY + 1 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY + 1 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &ExplosionVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, GrigioScuro);
    AudioMenu->add<Slider>("ExplosionSliderVolume", rect, pointer);

    // BLOCCO 6
    AudioMenu->add<Label>("labelPowerUpsAudio", Strings::PWVOLUME, nullptr, GameFont, 58, labelX + 18, centerY + 2 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY + 2 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, borderRadius, 0, 0, 100, &PowerUpVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, borderRadius, 0, GrigioScuro);
    AudioMenu->add<Slider>("PowerUpSliderVolume", rect, pointer);

    AudioMenu->add<Button>("Go Back Button", "Go Back", GameFont, fontSize, buttonWidth / 2, buttonHeight, centerX, (centerY + buttonWidth), [this]() {
        this->SetLayerPausedMenu();
        }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
}
void Interface::InitControlsSettings()
{
    JsonParser settings(PATH_SPACEINVADERS_SETTINGS);

    KeyBinds.KeyUP = settings.GetKey<int>("KeyBindings", "MOVEUP");
    KeyBinds.KeyDOWN = settings.GetKey<int>("KeyBindings", "MOVEDOWN");
    KeyBinds.KeyLEFT = settings.GetKey<int>("KeyBindings", "MOVELEFT");
    KeyBinds.KeyRIGHT = settings.GetKey<int>("KeyBindings", "MOVERIGHT");
    KeyBinds.KeySHOOT = settings.GetKey<int>("KeyBindings", "SHOOT");

    if (KeyBinds.KeyUP == 0 || KeyBinds.KeyDOWN == 0 || KeyBinds.KeyLEFT == 0 || KeyBinds.KeyRIGHT == 0 || KeyBinds.KeySHOOT == 0)
    {
        KeyBinds.KeyUP = KEY_W;
        KeyBinds.KeyLEFT = KEY_A;
        KeyBinds.KeyDOWN = KEY_S;
        KeyBinds.KeyRIGHT = KEY_D;
        KeyBinds.KeySHOOT = KEY_SPACE;
    }

    ControlsMenu->add<Button>("btnMoveUp", TextFormat(Strings::moveup, TranslateKey(KeyBinds.KeyUP)), GameFont, fontSize, buttonWidth, buttonHeight, centerX, yStart + (fontSize + spacing) * 0, [this]() { setToBind("btnMoveUp"); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("btnMoveLeft", TextFormat(Strings::moveleft, TranslateKey(KeyBinds.KeyLEFT)), GameFont, fontSize, buttonWidth, buttonHeight, centerX, yStart + (fontSize + spacing) * 1, [this]() { setToBind("btnMoveLeft"); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("btnMoveDown", TextFormat(Strings::movedown, TranslateKey(KeyBinds.KeyDOWN)), GameFont, fontSize, buttonWidth, buttonHeight, centerX, yStart + (fontSize + spacing) * 2, [this]() { setToBind("btnMoveDown"); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("btnMoveRight", TextFormat(Strings::moveright, TranslateKey(KeyBinds.KeyRIGHT)), GameFont, fontSize, buttonWidth, buttonHeight, centerX, yStart + (fontSize + spacing) * 3, [this]() { setToBind("btnMoveRight"); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("btnShoot", TextFormat(Strings::shoot, TranslateKey(KeyBinds.KeySHOOT)), GameFont, fontSize, buttonWidth, buttonHeight, centerX, yStart + (fontSize + spacing) * 4, [this]() { setToBind("btnShoot"); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("Reset", Strings::reset, GameFont, fontSize, buttonWidth, buttonHeight, centerX, yStart + (fontSize + spacing) * 5, [this]() { setToBind("Reset"); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);

    ControlsMenu->add<Button>("GoBackButton", "Go Back", GameFont, fontSize, buttonWidth / 2, buttonHeight, centerX, (centerY + buttonWidth), [this]() { this->SetLayerPausedMenu(); }, borderRadius, 0, 4, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
}

void Interface::SetLayerGame()
{
    *GameStatus = RUNNING;
    MainMenuHandler.PopMenu();
    MainMenuHandler.PushMenu(RunningMenu);
}
void Interface::SetLayerStart()
{
    *GameStatus = START;
    MainMenuHandler.PopMenu();
    MainMenuHandler.PushMenu(StartMenu);
}
void Interface::SetLayerPausedMenu(bool dead)
{
    MainMenuHandler.PopMenu();

    if (PausedMenu->getByID("btnRestart")->getActive())
    {
        if (dead)
        {
            PausedMenu->deactive("btnResume");
            PausedMenu->getByID("btnRestart")->setWidth(buttonWidth);
            PausedMenu->getByID("btnRestart")->setPosX(centerX);
        }
        else 
        {
            PausedMenu->activate("btnResume");
            PausedMenu->getByID("btnRestart")->setWidth(240);
            PausedMenu->getByID("btnRestart")->setPosX(centerX + (130));
        }
    }

    MainMenuHandler.PushMenu(PausedMenu);
}
void Interface::SetLayerControls()
{
    PreviousMenu = MainMenuHandler.TopMenu();
    MainMenuHandler.PushMenu(ControlsMenu);
}
void Interface::SetLayerAudio()
{
    PreviousMenu = MainMenuHandler.TopMenu();
    MainMenuHandler.PushMenu(AudioMenu);
}
void Interface::ToggleFullscreen()
{
    if (fullscreen)
    {
        SetWindowPosition(0, 0);
        SetWindowSize(BASE_WIDTH, BASE_HEIGHT - taskbar);
        PausedMenu->getByID("btnFullScreen")->setText(TextFormat(Strings::fullscreen, "Off"));
        fullscreen = false;
    }
    else
    {
        SetWindowPosition(0, 0);
        SetWindowSize(BASE_WIDTH, BASE_HEIGHT);
        PausedMenu->getByID("btnFullScreen")->setText(TextFormat(Strings::fullscreen, "On"));
        fullscreen = true;
    }
}

void Interface::UpdateSystem()
{
    MainMenuHandler.Update();
    MainMenuHandler.Draw();

    if ((IsKeyPressed(KEY_ESCAPE) || *GameStatus == KILLED) && *GameStatus != START)
    {
        bool dead = (*GameStatus == KILLED);
        *GameStatus = PAUSED;

        SetLayerPausedMenu(dead);
    }

    else if (*GameStatus == PAUSED)
    {
        updateKeyBinding();
    }

    if (MainMenuHandler.TopMenu() == PausedMenu)
    {
        if (ControlsMenu->getByID("btnMoveUp")->getText() == Strings::waitingKey)
            ControlsMenu->getByID("btnMoveUp")->setText(TextFormat(Strings::moveup, TranslateKey(KeyBinds.KeyUP)));

        if (ControlsMenu->getByID("btnMoveDown")->getText() == Strings::waitingKey)
            ControlsMenu->getByID("btnMoveDown")->setText(TextFormat(Strings::movedown, TranslateKey(KeyBinds.KeyDOWN)));

        if (ControlsMenu->getByID("btnMoveLeft")->getText() == Strings::waitingKey)
            ControlsMenu->getByID("btnMoveLeft")->setText(TextFormat(Strings::moveleft, TranslateKey(KeyBinds.KeyLEFT)));

        if (ControlsMenu->getByID("btnMoveRight")->getText() == Strings::waitingKey)
            ControlsMenu->getByID("btnMoveRight")->setText(TextFormat(Strings::moveright, TranslateKey(KeyBinds.KeyRIGHT)));

        if (ControlsMenu->getByID("btnShoot")->getText() == Strings::waitingKey)
            ControlsMenu->getByID("btnShoot")->setText(TextFormat(Strings::shoot, TranslateKey(KeyBinds.KeySHOOT)));
    }
}

std::shared_ptr<Menu> Interface::GetRunningMenu()
{
    return RunningMenu;
}
std::shared_ptr<Menu> Interface::GetStartMenu()
{
	return StartMenu;
}
std::shared_ptr<Menu> Interface::GetPausedMenu()
{
	return PausedMenu;
}
std::shared_ptr<Menu> Interface::GetAudioMenu()
{
	return AudioMenu;
}
std::shared_ptr<Menu> Interface::GetControlsMenu()
{
	return ControlsMenu;
}
