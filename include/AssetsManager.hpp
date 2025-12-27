#pragma once

#include <string>
#include <raylib.h>
#include <fstream>
#include <unordered_map>
#include "nlohmann/json.hpp"

class AssetsManager
{
public:
    static void Init(const std::string& jsonPath);
    static void Unload();
    ~AssetsManager()
    {
        Unload();
    }

    static Texture2D& GetTexture(const std::string& id);
    static Music& GetMusic(const std::string& id);
    static Sound& GetSound(const std::string& id);
    static Font& GetFont(const std::string& id);
    static Font& GetFontEx(const std::string& id, int fontSize, int* codepoints, int codepointCount);

private:
    inline static nlohmann::json assets;

    inline static std::unordered_map<std::string, Texture2D> textures;
    inline static std::unordered_map<std::string, Music> music;
    inline static std::unordered_map<std::string, Sound> sounds;
    inline static std::unordered_map<std::string, Font> fonts;
};