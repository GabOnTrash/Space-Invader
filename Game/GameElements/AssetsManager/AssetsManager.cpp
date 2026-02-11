#include "AssetsManager.hpp"

void AssetsManager::Init(const std::string& jsonPath)
{
    std::ifstream file(jsonPath);
    if (file.is_open())
    {
        file >> assets;
        file.close();
    }
}

Texture2D& AssetsManager::GetTexture(const std::string& id)
{
    if (textures.find(id) != textures.end())
        return textures[id];

    std::string path = assets["textures"][id].get<std::string>();
    textures[id] = LoadTexture(path.c_str());
    return textures[id];
}

Music& AssetsManager::GetMusic(const std::string& id)
{
    if (music.find(id) != music.end())
        return music[id];

    std::string path = assets["music"][id].get<std::string>();
    music[id] = LoadMusicStream(path.c_str());
    return music[id];
}

Sound& AssetsManager::GetSound(const std::string& id)
{
    if (sounds.find(id) != sounds.end())
        return sounds[id];

    std::string path = assets["sounds"][id].get<std::string>();
    sounds[id] = LoadSound(path.c_str());
    return sounds[id];
}

Font& AssetsManager::GetFont(const std::string& id)
{
    if (fonts.find(id) != fonts.end())
        return fonts[id];

    std::string path = assets["fonts"][id].get<std::string>();
    fonts[id] = LoadFont(path.c_str());
    return fonts[id];
}

Font& AssetsManager::GetFontEx(const std::string& id, int fontSize, int* codepoints, int codepointCount)
{
    std::string key = id + "_" + std::to_string(fontSize);

    if (fonts.find(key) != fonts.end())
        return fonts[key];

    std::string path = assets["fonts"][id].get<std::string>();
    fonts[key] = LoadFontEx(path.c_str(), fontSize, codepoints, codepointCount);
    return fonts[key];
}

void AssetsManager::Unload()
{
    for (auto& [_, t] : textures)   UnloadTexture(t);
    for (auto& [_, m] : music)      UnloadMusicStream(m);
    for (auto& [_, s] : sounds)     UnloadSound(s);
    for (auto& [_, f] : fonts)      UnloadFont(f);
}
