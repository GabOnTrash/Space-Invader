#pragma once

#include <nlohmann/json.hpp>

class JsonParser 
{
public:
    JsonParser(const std::string& filePath) 
    {
        this->settingsFilePath = filePath;
        LoadData();
    }

private:
    void LoadData()
    {
        std::ifstream file(settingsFilePath);

        if (file.is_open())
        {
            try
            {
                file >> settings;
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error while parsing JSON: " << e.what() << std::endl;
                settings = nlohmann::json::object(); // fallback a un oggetto vuoto
            }
        }
        else
        {
            std::cerr << "File not found, a new one will be created." << std::endl;
            settings = nlohmann::json::object();
        }
    }

public:
    void SaveData()
    {
        std::ofstream file(settingsFilePath);

        if (file.is_open())
        {
            file << settings.dump(4);
        }
        else
        {
            std::cerr << "Error while saving settings." << std::endl;
        }
    }

	template<typename T>
    T GetKey(const std::string& container, const std::string& type) const
    {
        if (settings.contains(container) && settings[container].contains(type))
        {
            return settings[container].at(type).get<T>();
        }
        return T{};
    }

	template<typename T>
    void SetKey(const std::string& container, const std::string& type, T value)
    {
		settings[container][type] = value;
    }

    void DeleteKey(const std::string& type)
    {
        if (settings.contains(type))
            settings.erase(type);
    }

private:
    std::string settingsFilePath;
    nlohmann::json settings;
};
