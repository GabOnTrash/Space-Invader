#include "SettingsManager.hpp"
#include "keys.hpp"
#include "AudioManager/AudioManager.hpp"

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

void SettingsManager::SaveData(const KeyBindings& KeyBinds)
{
    SetKey("audio", "GeneralVolume", AudioManager::GetGeneralVolume());
    SetKey("audio", "MusicVolume", AudioManager::GetMusicVolume());
    SetKey("audio", "LaserVolume", AudioManager::GetLaserVolume());
    SetKey("audio", "MeteorDamageVolume", AudioManager::GetMeteorDamageVolume());
    SetKey("audio", "PowerUpVolume", AudioManager::GetModifierVolume());
    SetKey("audio", "ExplosionVolume", AudioManager::GetExplosionVolume());
    SetKey("difficulty", "GameDifficulty", AudioManager::GetGameDifficulty());
    SetKey("KeyBindings", "MOVEUP", KeyBinds.KeyUP);
    SetKey("KeyBindings", "MOVEDOWN", KeyBinds.KeyDOWN);
    SetKey("KeyBindings", "MOVELEFT", KeyBinds.KeyLEFT);
    SetKey("KeyBindings", "MOVERIGHT", KeyBinds.KeyRIGHT);
    SetKey("KeyBindings", "SHOOT", KeyBinds.KeySHOOT);
    SetKey("KeyBindings", "DASH", KeyBinds.KeyDASH);
    SetKey("video", "Fullscreen", AudioManager::GetFullscreen());

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
