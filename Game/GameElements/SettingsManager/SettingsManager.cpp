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
    GenerateDefaultConfig();
    LoadVolumeValues();
}

void SettingsManager::LoadVolumeValues()
{
    AudioManager::Instance().generalVolume = GetKey<float>("audio", "GeneralVolume");
    AudioManager::Instance().musicVolume = GetKey<float>("audio", "MusicVolume");
    AudioManager::Instance().laserVolume = GetKey<float>("audio", "LaserVolume");
    AudioManager::Instance().meteorVolume = GetKey<float>("audio", "MeteorDamageVolume");
    AudioManager::Instance().modifierVolume = GetKey<float>("audio", "ModifierVolume");
    AudioManager::Instance().explosionVolume = GetKey<float>("audio", "ExplosionVolume");
}

void SettingsManager::SaveData(const KeyBindings& keys, MenuHandle& menu)
{
    SetKey("audio", "GeneralVolume", *AudioManager::Instance().getGeneralVolume());
    SetKey("audio", "MusicVolume", *AudioManager::Instance().getMusicVolume());
    SetKey("audio", "LaserVolume", *AudioManager::Instance().getLaserVolume());
    SetKey("audio", "MeteorDamageVolume", *AudioManager::Instance().getMeteorVolume());
    SetKey("audio", "ExplosionVolume", *AudioManager::Instance().getExplosionVolume());
    SetKey("audio", "ModifierVolume", *AudioManager::Instance().getModifierVolume());

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

void SettingsManager::GenerateDefaultConfig()
{
    SetKey<float>("audio", "GeneralVolume", 1.f);
    SetKey<float>("audio", "MusicVolume", 0.5f);
    SetKey<float>("audio", "LaserVolume", 0.5f);
    SetKey<float>("audio", "MeteorDamageVolume", 0.5f);
    SetKey<float>("audio", "ModifierVolume", 0.5f);
    SetKey<float>("audio", "ExplosionVolume", 0.5f);

    SetKey("difficulty", "GameDifficulty", 4);  // normal difficulty

    SetKey("KeyBindings", "UP", KEY_W);
    SetKey("KeyBindings", "DOWN", KEY_S);
    SetKey("KeyBindings", "LEFT", KEY_A);
    SetKey("KeyBindings", "RIGHT", KEY_D);
    SetKey("KeyBindings", "SHOOT", KEY_SPACE);
    SetKey("KeyBindings", "DASH", KEY_LEFT_SHIFT);

    SetKey("video", "Fullscreen", true);

    std::ofstream file(settingsPath);

    if (file.is_open())
        file << settings.dump(4);
}
