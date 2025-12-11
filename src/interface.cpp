#include "Interface.hpp"
#include "gioco.hpp"

KeyBindings KeyBinds;

Interface::Interface(GameState* GameStatus) : GameStatus(GameStatus)
{
}

void Interface::InitLayers()
{
    StartMenu = std::make_shared<Menu>();
    RunningMenu = std::make_shared<Menu>();
    PausedMenu = std::make_shared<Menu>();
    AudioMenu = std::make_shared<Menu>();
    ControlsMenu = std::make_shared<Menu>();

    GameFont = LoadFontEx("immagini/JBSemiBold.ttf", 100 * SCALE, nullptr, 0);
    SetTextureFilter(GameFont.texture, TEXTURE_FILTER_BILINEAR);

    InitStartMenuSettings();
    InitRunningMenuSettings();
    InitPausedMenuSettings();
    InitAudioControlSettings();
    InitControlsSettings();

    SettingsManager settings("SpaceInvadersSettings.json");

    GameDifficulty = static_cast<Difficulty>(settings.GetKey<int>("difficulty", "GameDifficulty"));

    if (GameDifficulty < 3 || GameDifficulty > 5)
    {
        GameDifficulty = NORMAL;
    }
    ChangeDifficulty();

    MainMenuHandler.PushMenu(StartMenu);
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
        KeyBinds.KeyDOWN = KEY_S;
        KeyBinds.KeyLEFT = KEY_A;
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
    StartMenu->add<Label>("labelTitolo", Strings::title, nullptr, GameFont, 80 * SCALE, WINDOW_WIDTH / 2 + (WINDOW_WIDTH / 100) * SCALE, 100 * SCALE, GrigioChiaro, GrigioScuro);
    StartMenu->add<Button>("bottoneStart", Strings::start, GameFont, 50 * SCALE, 500 * SCALE, 120 * SCALE, WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2 - 70 * SCALE), [this]() { if (CallRestart) this->CallStart(); }, 0.0f, 10, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    StartMenu->add<Button>("difficButton", TextFormat(Strings::difficulty, TranslateToDifficulty()), GameFont, 50 * SCALE, 500 * SCALE, 120 * SCALE, WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2 + 70 * SCALE), [this]() { this->SetDifficulty(); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
}
void Interface::InitRunningMenuSettings()
{
    RunningMenu->add<Label>("labelReady", Strings::ready, nullptr, GameFont, 70 * SCALE, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WHITE, WHITE);
    RunningMenu->add<Label>("labelScore", Strings::score, [this]() { if (CallGetScore) { return this->CallGetScore(); }; }, GameFont, 40 * SCALE, WINDOW_WIDTH / 10, WINDOW_HEIGHT / 35, WHITE, WHITE);
    RunningMenu->add<Label>("labelDanger", Strings::danger, nullptr, GameFont, 70 * SCALE, WINDOW_WIDTH - offsetY * 1.5f * SCALE, offsetY / 2 * SCALE, WHITE, ORANGE);
}
void Interface::InitPausedMenuSettings()
{
    PausedMenu->add<Button>("btnResume", Strings::resume, GameFont, 50 * SCALE, 240 * SCALE, 120 * SCALE, (WINDOW_WIDTH / 2 - 130 * SCALE), (WINDOW_HEIGHT / 2 - 200 * SCALE), [this]() { if (CallResume) this->CallResume(); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    PausedMenu->add<Button>("btnRestart", Strings::restart, GameFont, 50 * SCALE, 240 * SCALE, 120 * SCALE, (WINDOW_WIDTH / 2 + 130 * SCALE), (WINDOW_HEIGHT / 2 - 200 * SCALE), [this]() { if (CallRestart) this->CallRestart(); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);

    PausedMenu->add<Button>("btnctrl", Strings::control, GameFont, 50 * SCALE, 240 * SCALE, 120 * SCALE, (WINDOW_WIDTH / 2 - 130 * SCALE), (WINDOW_HEIGHT / 2 - 60 * SCALE), [this]() { this->SetLayerControls(); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    PausedMenu->add<Button>("btnAudio", Strings::audioSettings, GameFont, 50 * SCALE, 240 * SCALE, 120 * SCALE, (WINDOW_WIDTH / 2 + 130 * SCALE), (WINDOW_HEIGHT / 2 - 60 * SCALE), [this]() { this->SetLayerAudio(); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);

    PausedMenu->add<Button>("difficButton2", TextFormat(Strings::difficulty, TranslateToDifficulty()), GameFont, 50 * SCALE, 500 * SCALE, 120 * SCALE, WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2 + 80 * SCALE), [this]() { this->SetDifficulty(); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    PausedMenu->add<Button>("btnQuit", Strings::quit, GameFont, 50 * SCALE, 500 * SCALE, 120 * SCALE, WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2 + 220 * SCALE), [this]() { this->SetLayerStart(); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
}
void Interface::InitAudioControlSettings()
{
    SettingsManager settings("SpaceInvadersSettings.json");

    GeneralVolume = settings.GetKey<float>("audio", "GeneralVolume");
    MusicVolume = settings.GetKey<float>("audio", "MusicVolume");
    LaserVolume = settings.GetKey<float>("audio", "LaserVolume");
    AsteroidVolume = settings.GetKey<float>("audio", "AsteroidVolume");
    ExplosionVolume = settings.GetKey<float>("audio", "ExplosionVolume");
    PowerUpVolume = settings.GetKey<float>("audio", "PowerUpVolume");

    centerY = WINDOW_HEIGHT / 2;
    blockSpacing = 140 * SCALE;
    sliderOffset = 50 * SCALE;
    labelX = WINDOW_WIDTH / 2;
    sliderX = WINDOW_WIDTH / 2;
    sliderWidth = 400 * SCALE;
    sliderHeight = 60 * SCALE;
    pointerWidth = 20 * SCALE;

    // BLOCCO 1
    AudioMenu->add<Label>("labelGeneralAudio", Strings::GENVOLUME, nullptr, GameFont, 60 * SCALE, labelX, centerY - 3 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY - 3 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, 0.0f, 0, 0, 1, &GeneralVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, 0.0f, 0, GrigioScuro);
    AudioMenu->add<Slider>("GeneralSliderVolume", rect, pointer);

    // BLOCCO 2
    AudioMenu->add<Label>("labelMusicAudio", Strings::MUSCVOLUME, nullptr, GameFont, 58 * SCALE, labelX + 7 * SCALE, centerY - 2 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY - 2 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, 0.0f, 0, 0, 100, &MusicVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, 0.0f, 0, GrigioScuro);
    AudioMenu->add<Slider>("MusicSliderVolume", rect, pointer);

    // BLOCCO 3
    AudioMenu->add<Label>("labelLaserAudio", Strings::LASVOLUME, nullptr, GameFont, 67 * SCALE, labelX + 8 * SCALE, centerY - 1 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY - 1 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, 0.0f, 0, 0, 100, &LaserVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, 0.0f, 0, GrigioScuro);
    AudioMenu->add<Slider>("LaserSliderVolume", rect, pointer);

    // BLOCCO 4
    AudioMenu->add<Label>("labelAsteoridAudio", Strings::ASTVOLUME, nullptr, GameFont, 54 * SCALE, labelX + 4 * SCALE, centerY + 0 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY + 0 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, 0.0f, 0, 0, 100, &AsteroidVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, 0.0f, 0, GrigioScuro);
    AudioMenu->add<Slider>("AsteroidSliderVolume", rect, pointer);

    // BLOCCO 5
    AudioMenu->add<Label>("labelExplosionAudio", Strings::EXPLVOLUME, nullptr, GameFont, 51 * SCALE, labelX - 2 * SCALE, centerY + 1 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY + 1 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, 0.0f, 0, 0, 100, &ExplosionVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, 0.0f, 0, GrigioScuro);
    AudioMenu->add<Slider>("ExplosionSliderVolume", rect, pointer);

    // BLOCCO 6
    AudioMenu->add<Label>("labelPowerUpsAudio", Strings::PWVOLUME, nullptr, GameFont, 58 * SCALE, labelX + 18 * SCALE, centerY + 2 * blockSpacing, GrigioChiaro, GrigioScuro);
    rect = RectS(sliderX, centerY + 2 * blockSpacing + sliderOffset, sliderWidth, sliderHeight, 0.0f, 0, 0, 100, &PowerUpVolume, GrigioChiaro);
    pointer = PointerS(true, pointerWidth, sliderHeight, 0.0f, 0, GrigioScuro);
    AudioMenu->add<Slider>("PowerUpSliderVolume", rect, pointer);

    AudioMenu->add<Button>("Go Back Button", "Go Back", GameFont, 50 * SCALE, 200 * SCALE, 100 * SCALE, WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2 + 500 * SCALE), [this]() {
        this->SetLayerPausedMenu();
        }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
}
void Interface::InitControlsSettings()
{
    SettingsManager settings("SpaceInvadersSettings.json");

    KeyBinds.KeyUP = settings.GetKey<int>("KeyBindings", "MOVEUP");
    KeyBinds.KeyDOWN = settings.GetKey<int>("KeyBindings", "MOVEDOWN");
    KeyBinds.KeyLEFT = settings.GetKey<int>("KeyBindings", "MOVELEFT");
    KeyBinds.KeyRIGHT = settings.GetKey<int>("KeyBindings", "MOVERIGHT");
    KeyBinds.KeySHOOT = settings.GetKey<int>("KeyBindings", "SHOOT");

    if (KeyBinds.KeyUP == 0 || KeyBinds.KeyDOWN == 0 || KeyBinds.KeyLEFT == 0 || KeyBinds.KeyRIGHT == 0 || KeyBinds.KeySHOOT == 0)
    {
        KeyBinds.KeyUP = KEY_W;
        KeyBinds.KeyDOWN = KEY_S;
        KeyBinds.KeyLEFT = KEY_A;
        KeyBinds.KeyRIGHT = KEY_D;
        KeyBinds.KeySHOOT = KEY_SPACE;
    }

    buttonHeight = 50 * SCALE;
    spacing = 90 * SCALE;
    totalHeight = (buttonHeight * 6) + (spacing * 5);
    yStart = (WINDOW_HEIGHT / 2) - (totalHeight / 2);
    centerX = WINDOW_WIDTH / 2;

    ControlsMenu->add<Button>("btnMoveUp", TextFormat(Strings::moveup, TranslateKey(KeyBinds.KeyDOWN)), GameFont, buttonHeight, 500 * SCALE, 120 * SCALE, centerX, yStart + (buttonHeight + spacing) * 0, [this]() { setToBind("btnMoveUp"); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("btnMoveDown", TextFormat(Strings::movedown, TranslateKey(KeyBinds.KeyDOWN)), GameFont, buttonHeight, 500 * SCALE, 120 * SCALE, centerX, yStart + (buttonHeight + spacing) * 1, [this]() { setToBind("btnMoveDown"); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("btnMoveLeft", TextFormat(Strings::moveleft, TranslateKey(KeyBinds.KeyLEFT)), GameFont, buttonHeight, 500 * SCALE, 120 * SCALE, centerX, yStart + (buttonHeight + spacing) * 2, [this]() { setToBind("btnMoveLeft"); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("btnMoveRight", TextFormat(Strings::moveright, TranslateKey(KeyBinds.KeyRIGHT)), GameFont, buttonHeight, 500 * SCALE, 120 * SCALE, centerX, yStart + (buttonHeight + spacing) * 3, [this]() { setToBind("btnMoveRight"); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("btnShoot", TextFormat(Strings::shoot, TranslateKey(KeyBinds.KeySHOOT)), GameFont, buttonHeight, 500 * SCALE, 120 * SCALE, centerX, yStart + (buttonHeight + spacing) * 4, [this]() { setToBind("btnShoot"); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
    ControlsMenu->add<Button>("Reset", Strings::reset, GameFont, buttonHeight, 500 * SCALE, 120 * SCALE, centerX, yStart + (buttonHeight + spacing) * 5, [this]() { setToBind("Reset"); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);

    ControlsMenu->add<Button>("Go Back Button", "Go Back", GameFont, 50 * SCALE, sliderWidth - 4 * SCALE, 100 * SCALE, WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2 + 500 * SCALE), [this]() { this->SetLayerPausedMenu(); }, 0.0f, 0, 4 * SCALE, WHITE, GrigioScuro, GrigioScuro, GrigioScuro, WHITE);
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
void Interface::SetLayerPausedMenu()
{
    MainMenuHandler.PopMenu();

    if (PausedMenu->getByID("btnRestart")->getActive())
    {
        if (*GameStatus == KILLED)
        {
            PausedMenu->deactive("btnResume");
            PausedMenu->getByID("btnRestart")->setWidth(500 * SCALE);
            PausedMenu->getByID("btnRestart")->setPosX(WINDOW_WIDTH / 2);
        }
        else if (*GameStatus == PAUSED)
        {
            PausedMenu->activate("btnResume");
            PausedMenu->getByID("btnRestart")->setWidth(240 * SCALE);
            PausedMenu->getByID("btnRestart")->setPosX(WINDOW_WIDTH / 2 + (130 * SCALE));
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

void Interface::UpdateSystem()
{
    MainMenuHandler.Update();
    MainMenuHandler.Draw();

    if ((IsKeyPressed(KEY_ESCAPE) || *GameStatus == KILLED) && *GameStatus != START)
    {
        if (*GameStatus != KILLED)
            *GameStatus = PAUSED;

        SetLayerPausedMenu();
    }

    if (*GameStatus == PAUSED)
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
