#pragma once

#include <fstream>
#include "nlohmann/json.hpp"
#include "../AudioManager/AudioManager.hpp"
#include "../MenuHandle/MenuHandle.hpp"

class AudioManager;
class MenuHandle;

class SettingsManager
{
public:
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
    inline static nlohmann::json settings;
    inline static std::string settingsPath;
};
