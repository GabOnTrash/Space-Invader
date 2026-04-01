#include "SettingsManager.hpp"
#include "GameElements/util/Logger.hpp"

void SettingsManager::Init(const std::string& jsonPath)
{
    settingsPath = jsonPath;

    std::ifstream file(settingsPath);
    if (file.is_open())
    {
        file >> settings;
        file.close();
        LoadVolumeValues();
        return;
    }

    LOG_INFO_FILE("File not found, a new one will be created.");
    settings = nlohmann::json::object();
}

void SettingsManager::LoadVolumeValues()
{
    AudioManager::Instance().setGlobalVolume(GetKey<float>("audio", "GeneralVolume"));
    AudioManager::Instance().setMusicVolume(GetKey<float>("audio", "MusicVolume"));
    AudioManager::Instance().setLaserVolume(GetKey<float>("audio", "LaserVolume"));
    AudioManager::Instance().setMeteorVolume(GetKey<float>("audio", "MeteorDamageVolume"));
    AudioManager::Instance().setModifierVolume(GetKey<float>("audio", "ModifierVolume"));
    AudioManager::Instance().setExplosionVolume(GetKey<float>("audio", "ExplosionVolume"));
}

void SettingsManager::SaveData(const KeyBindings& keys, MenuHandle& menu)
{
    SetKey("audio", "GeneralVolume", *AudioManager::Instance().getGlobalVolume());
    SetKey("audio", "MusicVolume", *AudioManager::Instance().getMusicVolume());
    SetKey("audio", "LaserVolume", *AudioManager::Instance().getLaserVolume());
    SetKey("audio", "MeteorDamageVolume", *AudioManager::Instance().getMeteorVolume());
    SetKey("audio", "ModifierVolume", *AudioManager::Instance().getModifierVolume());
    SetKey("audio", "ExplosionVolume", *AudioManager::Instance().getExplosionVolume());

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
