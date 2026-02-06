#include "AudioManager.hpp"

void AudioManager::Init()
{
    globalVolume = SettingsManager::GetKey<float>("audio", "GeneralVolume");
    musicVolume = SettingsManager::GetKey<float>("audio", "MusicVolume");
    laserVolume = SettingsManager::GetKey<float>("audio", "LaserVolume");
    meteorVolume = SettingsManager::GetKey<float>("audio", "MeteorDamageVolume");
    explosionVolume = SettingsManager::GetKey<float>("audio", "ExplosionVolume");
    modifierVolume = SettingsManager::GetKey<float>("audio", "ModifierVolume");
}
void AudioManager::Update()
{
    SetMusicVolume(musicVolume);
}
