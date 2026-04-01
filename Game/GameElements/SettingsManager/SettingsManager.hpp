#pragma once

#include <fstream>
#include "nlohmann/json.hpp"
#include "../MenuHandle/MenuHandle.hpp"

class MenuHandle;

class AudioManager
{
public:
    static AudioManager& Instance()
    {
        static AudioManager instance;
        return instance;
    }

    [[nodiscard]] float* getMusicVolume() { return &musicVolume; }
    [[nodiscard]] float* getGeneralVolume() { return &generalVolume; }
    [[nodiscard]] float* getExplosionVolume() { return &explosionVolume; }
    [[nodiscard]] float* getModifierVolume() { return &modifierVolume; }
    [[nodiscard]] float* getMeteorVolume() { return &meteorVolume; }
    [[nodiscard]] float* getLaserVolume() { return &laserVolume; }

    [[nodiscard]] float getCalibratedMusicVolume() const { return musicVolume * generalVolume; }
    [[nodiscard]] float getCalibratedExplosionVolume() const { return explosionVolume * generalVolume; }
    [[nodiscard]] float getCalibratedModifierVolume() const { return modifierVolume * generalVolume; }
    [[nodiscard]] float getCalibratedMeteorVolume() const { return meteorVolume * generalVolume; }
    [[nodiscard]] float getCalibratedLaserVolume() const { return laserVolume * generalVolume; }

private:
    float generalVolume = 1.f;
    float musicVolume = 1.f;
    float explosionVolume = 1.f;
    float laserVolume = 1.f;
    float modifierVolume = 1.f;
    float meteorVolume = 1.f;

    friend class SettingsManager;
};

class SettingsManager
{
public:
    static void LoadVolumeValues();
    static void Init(const std::string& jsonPath);
    static void SaveData(const KeyBindings& keys, MenuHandle& menu);
    static void DeleteKey(const std::string& type);

    // audio manager calls this to set up the initial volumes
	template<typename T>
    static T GetKey(const std::string& container, const std::string& type)
    {
        if (settings.contains(container) && settings[container].contains(type))
        {
            return settings[container].at(type).get<T>();
        }
        return T{};
    }

	template<typename T>
    static void SetKey(const std::string& container, const std::string& type, T value)
    {
		settings[container][type] = value;
    }

private:
    static void GenerateDefaultConfig();

    inline static nlohmann::json settings;
    inline static std::string settingsPath;
};
