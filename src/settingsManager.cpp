#include "settingsManager.hpp"

SettingsManager::SettingsManager(){}
SettingsManager::SettingsManager(const std::string& filePath)
{
    this->settingsFilePath = GetFullPath(filePath);

    LoadData();
}

std::string SettingsManager::GetFullPath(const std::string& fileName)
{
    char* buffer = nullptr;
    size_t size = 0;

    if (_dupenv_s(&buffer, &size, "USERPROFILE") != 0 || buffer == nullptr)
    {
        std::cerr << "Error trying to get USERPROFILE";
        return {};
    }

    std::filesystem::path userPath(buffer);
    free(buffer);

    std::filesystem::path fullPath = userPath / fileName;
    
    return fullPath.string();
}
void SettingsManager::LoadData() 
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
            std::cerr << "Errore nel parsing del file JSON: " << e.what() << std::endl;
            settings = nlohmann::json::object();  // fallback a un oggetto vuoto
        }
    }
    else 
    {
        std::cerr << "File di impostazioni non trovato. Verrà creato uno nuovo." << std::endl;
        settings = nlohmann::json::object();
    }
}
void SettingsManager::SaveData() const 
{
    std::ofstream file(settingsFilePath);

    if (file.is_open()) 
    {
        file << settings.dump(4);  // 4 spazi di indentazione
    }
    else 
    {
        std::cerr << "Errore nel salvataggio delle impostazioni." << std::endl;
    }
}

//void SettingsManager::SaveMap(const std::map<const char*, float>& Map, const std::string& type)
//{
//    for (const auto& [key, ptr] : Map)
//    {
//        settings[type][key] = ptr;
//    }
//}
//void SettingsManager::LoadMap(const std::map<const char*, float>& Map, const std::string& type)
//{
//    /*if (settings.contains(type))
//    {
//        for (auto& [key, ptr] : Map)
//        {
//            if (settings[type].contains(key))
//            {
//                ptr = settings[type][key].get<float>();
//            }
//        }
//    }*/
//}
//float SettingsManager::GetKey(const std::string& container, const std::string& type) const
//{
//    if (settings.contains(container) && settings[container].contains(type)) 
//    {
//        return settings[container].at(type).get<float>();
//    }
//    return 1.0f;  // valore di default
//}
//void SettingsManager::SetKey(const std::string& container, const std::string& type, float value)
//{
//    settings[container][type] = value;
//}

void SettingsManager::DeleteKey(const std::string& section)
{
    if (settings.contains(section)) settings.erase(section);
}