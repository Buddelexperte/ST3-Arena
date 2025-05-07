#pragma once
#include <iostream>
#include <memory>

#include "SoundManager.h"

SoundManager::SoundManager()
{

}

void SoundManager::play(const sf::SoundBuffer& buffer, const ESoundEnv sound_env)
{
    SoundEnvironment newSound(std::make_unique<sf::Sound>(buffer), sound_env); // Create new Sound Object
    float usedVolume = getActualVolume(sound_env);

    newSound.sound->setVolume(usedVolume);
    newSound.sound->play(); // Play the sound immediately

    activeSounds_master.push_back(std::move(newSound));
}

void SoundManager::cleanUp()
{
    // Remove all sounds that are not playing anymore
    activeSounds_master.erase(
        std::remove_if(activeSounds_master.begin(), activeSounds_master.end(),
            [](const SoundEnvironment& se) {
                return se.sound->getStatus() == sf::Sound::Stopped;
            }),
        activeSounds_master.end());
}

float SoundManager::getMasterVolume() const
{
    return volume_master;
}

void SoundManager::setMasterVolume(const float newVolume)
{
    volume_master = newVolume;
}

float SoundManager::getVolumeFactor(const ESoundEnv sound_env) const
{
    switch (sound_env)
    {
    case ESoundEnv::UI:
        return volume_ui_f;
    case ESoundEnv::GAMEPLAY:
        return volume_gameplay_f;
    case ESoundEnv::MUSIC:
        return volume_music_f;
    case ESoundEnv::MASTER:
    default:
        break;
    }

    return volume_master / 100.0f;
}

void SoundManager::setVolumeFactor(const float f, const ESoundEnv sound_env)
{
    switch (sound_env)
    {
    case ESoundEnv::UI:
        volume_ui_f = f;
        break;
    case ESoundEnv::GAMEPLAY:
        volume_gameplay_f = f;
        break;
    case ESoundEnv::MUSIC:
        volume_music_f = f;
        break;
    case ESoundEnv::MASTER:
    default:
        volume_master = f * 100.0f;
        break;
    }
}

bool SoundManager::getIsMuted(const ESoundEnv sound_env) const
{
    switch (sound_env)
    {
    case ESoundEnv::UI:
        return bMuted_ui;
    case ESoundEnv::GAMEPLAY:
        return bMuted_gameplay;
    case ESoundEnv::MUSIC:
        return bMuted_music;
    case ESoundEnv::MASTER:
    default:
        break;
    }

    return bMuted_master;
}

void SoundManager::setIsMuted(const bool newMuted, const ESoundEnv sound_env)
{
    switch (sound_env)
    {
    case ESoundEnv::UI:
        bMuted_ui = newMuted;
        break;
    case ESoundEnv::GAMEPLAY:
        bMuted_gameplay = newMuted;
        break;
    case ESoundEnv::MUSIC:
        bMuted_music = newMuted;
        break;
    case ESoundEnv::MASTER:
    default:
        bMuted_master = newMuted;
        break;
    }

}

float SoundManager::getActualVolume(ESoundEnv sound_env) const
{
    if (bMuted_master)
        return 0.0f;

    switch (sound_env)
    {
    case ESoundEnv::UI:
        if (bMuted_ui)
            return 0.0f;
        return volume_master * volume_ui_f;
    case ESoundEnv::GAMEPLAY:
        if (bMuted_gameplay)
            return 0.0f;
        return bMuted_gameplay * volume_gameplay_f;
    case ESoundEnv::MUSIC:
        if (bMuted_music)
            return 0.0f;
        return bMuted_music * volume_music_f;
    case ESoundEnv::MASTER:
    default:
        break;
    }

    return volume_master;
}

void SoundManager::updateOnSoundSettings()
{
    for (auto& elem : activeSounds_master)
    {
        elem.sound->setVolume(getActualVolume(elem.environment));
    }
}

const sf::SoundBuffer& SoundManager::getSound_Click()
{
    if (!sound_click.bLoaded)
    {
        if (!sound_click.sound.loadFromFile("Content/Sounds/Glitch.wav"))
        {
            std::cerr << "Unable to load click sound!" << std::endl;
        }
        sound_click.bLoaded = true;
    }
    return sound_click.sound;
}

const sf::SoundBuffer& SoundManager::getSound_ReturnClick()
{
    if (!sound_returnClick.bLoaded)
    {
        if (!sound_returnClick.sound.loadFromFile("Content/Sounds/Glitch2.wav"))
        {
            std::cerr << "Unable to load click return sound!" << std::endl;
        }
        sound_returnClick.bLoaded = true;
    }
    return sound_returnClick.sound;
}