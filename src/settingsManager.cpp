#include "settingsManager.hpp"

SettingsManager::SettingsManager(const std::string& filePath, Interface& MenuSystem) 
    : MenuSystem(MenuSystem)
{
    this->settingsFilePath = GetFullPath(filePath);

    LoadData();
}
SettingsManager::SettingsManager(const std::string& filePath) 
    : MenuSystem(*(new Interface()))
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
void SettingsManager::GetData()
{
    SetKey("audio", "GeneralVolume", MenuSystem.GetGeneralVolume());
    SetKey("audio", "MusicVolume", MenuSystem.GetMusicVolume());
    SetKey("audio", "LaserVolume", MenuSystem.GetLaserVolume());
    SetKey("audio", "AsteroidVolume", MenuSystem.GetAsteroidVolume());
    SetKey("audio", "PowerUpVolume", MenuSystem.GetPowerUpVolume());
    SetKey("audio", "ExplosionVolume", MenuSystem.GetExplosionVolume());

    SetKey("difficulty", "GameDifficulty", MenuSystem.GetGameDifficulty());

    SetKey("KeyBindings", "MOVEUP", KeyBinds.KeyUP);
    SetKey("KeyBindings", "MOVEDOWN", KeyBinds.KeyDOWN);
    SetKey("KeyBindings", "MOVELEFT", KeyBinds.KeyLEFT);
    SetKey("KeyBindings", "MOVERIGHT", KeyBinds.KeyRIGHT);
    SetKey("KeyBindings", "SHOOT", KeyBinds.KeySHOOT);

    SetKey("video", "Fullscreen", MenuSystem.GetFullscreen());
}

void SettingsManager::SaveData()
{
    GetData();
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

void SettingsManager::DeleteKey(const std::string& section)
{
    if (settings.contains(section))
        settings.erase(section);
}