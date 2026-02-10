#include "SettingsManager.hpp"

void SettingsManager::Init(const std::string& jsonPath)
{
    settingsPath = jsonPath;

    std::ifstream file(settingsPath);
    if (file.is_open())
    {
        file >> settings;
        file.close();
        return;
    }

    std::cerr << "File not found, a new one will be created." << std::endl;
    settings = nlohmann::json::object();
}

void SettingsManager::SaveData(const GameContext& ctx, MenuHandle& menu)
{
    SetKey("audio", "GeneralVolume", AudioManager::GetGeneralVolume());
    SetKey("audio", "MusicVolume", AudioManager::GetMusicVolume());
    SetKey("audio", "LaserVolume", AudioManager::GetLaserVolume());
    SetKey("audio", "MeteorDamageVolume", AudioManager::GetMeteorDamageVolume());
    SetKey("audio", "PowerUpVolume", AudioManager::GetModifierVolume());
    SetKey("audio", "ExplosionVolume", AudioManager::GetExplosionVolume());
    SetKey("difficulty", "GameDifficulty", menu.GameDifficulty);
    SetKey("KeyBindings", "MOVEUP", ctx.keyBindings.KeyUP);
    SetKey("KeyBindings", "MOVEDOWN", ctx.keyBindings.KeyDOWN);
    SetKey("KeyBindings", "MOVELEFT", ctx.keyBindings.KeyLEFT);
    SetKey("KeyBindings", "MOVERIGHT", ctx.keyBindings.KeyRIGHT);
    SetKey("KeyBindings", "SHOOT", ctx.keyBindings.KeySHOOT);
    SetKey("KeyBindings", "DASH", ctx.keyBindings.KeyDASH);
    SetKey("video", "Fullscreen", menu.IsFullscreen());

    std::ofstream file(settingsPath);

    if (file.is_open())
        file << settings.dump(4);
    
    else
        std::cerr << "Error while saving settings." << std::endl;
}

void SettingsManager::DeleteKey(const std::string& type)
{
    if (settings.contains(type))
        settings.erase(type);
}
