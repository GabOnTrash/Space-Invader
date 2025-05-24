#pragma once

#include <nlohmann/json.hpp>

#include "specs.hpp"

class SettingsManager 
{
public:

    SettingsManager();
    SettingsManager(const std::string& filePath);

    void LoadData();
    void SaveData() const;

    // Volume
    //void SaveMap(const std::map<const char*, float>& Map, const std::string& type);
    //void LoadMap(const std::map<const char*, float>& Map, const std::string& type);

	template<typename T>
    T GetKey(const std::string& container, const std::string& type) const
    {
        if (settings.contains(container) && settings[container].contains(type))
        {
            return settings[container].at(type).get<T>();
        }
        return T{};  // valore di default
    }

	template<typename T>
    void SetKey(const std::string& container, const std::string& type, T value)
    {
		settings[container][type] = value;
    }

    void DeleteKey(const std::string& type);

private:

    std::string GetFullPath(const std::string& fileName);

    std::string settingsFilePath;
    nlohmann::json settings;
};
