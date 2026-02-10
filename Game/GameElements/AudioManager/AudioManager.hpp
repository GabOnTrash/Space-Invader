#pragma once
#include <memory>
#include "../MenuHandle/MenuHandle.hpp"
#include "../SettingsManager/SettingsManager.hpp"

class AudioManager
{
public:
    static void Init();
    static void Update();

    // settings manager calls these to save the volumes in the file
    static float GetGeneralVolume() { return globalVolume; };
    static float GetMusicVolume() { return musicVolume; };
    static float GetLaserVolume() { return laserVolume; };
    static float GetModifierVolume() { return modifierVolume; };
    static float GetMeteorDamageVolume() { return meteorVolume; };
    static float GetExplosionVolume() { return explosionVolume; };

    // menu calls these to set up the settings page
    static void SetGeneralVolume(float volume) { globalVolume = volume; };
    static void SetMusicVolume(float volume) { musicVolume = volume; };
    static void SetLaserVolume(float volume) { laserVolume = volume; };
    static void SetModifierVolume(float volume) { modifierVolume = volume; };
    static void SetMeteorDamageVolume(float volume) { meteorVolume = volume; };
    static void SetExplosionVolume(float volume) { explosionVolume = volume; };

private:
    inline static float globalVolume = 1.f;
    inline static float musicVolume = 1.f;
    inline static float explosionVolume = 1.f;
    inline static float laserVolume = 1.f;
    inline static float modifierVolume = 1.f;
    inline static float meteorVolume = 1.f;
};
