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

void SettingsManager::SaveData(const KeyBindings& keys, MenuHandle& menu)
{
    SetKey("audio", "GeneralVolume", AudioManager::GetGeneralVolume());
    SetKey("audio", "MusicVolume", AudioManager::GetMusicVolume());
    SetKey("audio", "LaserVolume", AudioManager::GetLaserVolume());
    SetKey("audio", "MeteorDamageVolume", AudioManager::GetMeteorDamageVolume());
    SetKey("audio", "PowerUpVolume", AudioManager::GetModifierVolume());
    SetKey("audio", "ExplosionVolume", AudioManager::GetExplosionVolume());
    SetKey("difficulty", "GameDifficulty", menu.GameDifficulty);

    keys.forEach([](const std::string& key, int val)
    {
        SetKey("KeyBindings", key, val);
    });

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
