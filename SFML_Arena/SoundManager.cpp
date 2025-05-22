#pragma once
#include <iostream>
#include <memory>

#include "SoundManager.h"
#include "GameInstance.h"

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

void SoundManager::playMusic(const std::string& filePath)
{
    if (!backgroundMusic.openFromFile(filePath))
    {
        std::cerr << "Unable to load music: " << filePath << std::endl;
        return;
    }

    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(getActualVolume(ESoundEnv::MUSIC));
    backgroundMusic.play();
}

void SoundManager::pauseMusic()
{
    if (backgroundMusic.getStatus() == sf::SoundSource::Playing)
        backgroundMusic.pause();
}

void SoundManager::resumeMusic()
{
    if (backgroundMusic.getStatus() == sf::SoundSource::Paused)
        backgroundMusic.play();
}

void SoundManager::stopMusic()
{
    backgroundMusic.stop();
}

void SoundManager::pauseGameplaySound()
{
    gameplaySoundsPaused = true;

    for (auto& elem : activeSounds_master)
    {
        if (elem.environment != ESoundEnv::GAMEPLAY)
            continue;

        elem.sound->pause();
    }

    updateOnSoundSettings();
}

void SoundManager::resumeGameplaySound()
{
    gameplaySoundsPaused = false;

    for (auto& elem : activeSounds_master)
    {
        if (elem.environment != ESoundEnv::GAMEPLAY)
            continue;

        elem.sound->play();
    }

    updateOnSoundSettings();
}

float SoundManager::getMasterVolume() const
{
    return volume_master;
}

void SoundManager::setMasterVolume(const float newVolume)
{
    volume_master = newVolume;

    updateOnSoundSettings();
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
        volume_music_f_paused = volume_music_f * 0.7f;
        break;
    case ESoundEnv::MASTER:
    default:
        volume_master = f * 100.0f;
        break;
    }

    updateOnSoundSettings();
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

    updateOnSoundSettings();
}

float SoundManager::getActualVolume(ESoundEnv sound_env) const
{
    static constexpr float UI_MULTIPLIER = 0.4f; // Music is too quiet and doesn't fit 0-1 scale
    static constexpr float MUSIC_MULTIPLIER = 4.0f; // Music is too quiet and doesn't fit 0-1 scale
    static constexpr float GAMEPLAY_MULTIPLIER = 5.0f; // Music is too quiet and doesn't fit 0-1 scale

    if (bMuted_master)
        return 0.0f;

    switch (sound_env)
    {
    case ESoundEnv::UI:
        if (bMuted_ui)
            return 0.0f;
        return volume_master * volume_ui_f * UI_MULTIPLIER;
    case ESoundEnv::GAMEPLAY:
        if (bMuted_gameplay)
            return 0.0f;
        return !bMuted_gameplay * volume_gameplay_f * GAMEPLAY_MULTIPLIER;
    case ESoundEnv::MUSIC:
        if (bMuted_music)
            return 0.0f;
        return !bMuted_music * (gameplaySoundsPaused ? volume_music_f_paused : volume_music_f) * MUSIC_MULTIPLIER;
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

    backgroundMusic.setVolume(getActualVolume(ESoundEnv::MUSIC));
}

const sf::SoundBuffer& SoundManager::getSound_Click()
{
    static const std::string path = "Content/Sounds/Menu_Click.wav"; // Old click sound: "Content/Sounds/Glitch.wav"

    if (!sound_click.bLoaded)
    {
        if (!sound_click.sound.loadFromFile(path))
        {
            std::cerr << "Unable to load sound from path: " << path << std::endl;
        }
        sound_click.bLoaded = true;
    }
    return sound_click.sound;
}

const sf::SoundBuffer& SoundManager::getSound_ReturnClick()
{
    static const std::string path = "Content/Sounds/Menu_Close.wav"; // Old click sound: "Content/Sounds/Glitch2.wav"
 
    if (!sound_returnClick.bLoaded)
    {
        if (!sound_returnClick.sound.loadFromFile(path))
        {
            std::cerr << "Unable to load sound from path: " << path << std::endl;
        }
        sound_returnClick.bLoaded = true;
    }
    return sound_returnClick.sound;
}

const sf::SoundBuffer& SoundManager::getSound_LevelUp()
{
    static const std::string path = "Content/Sounds/Level_Up.wav";

    if (!sound_LevelUp.bLoaded)
    {
        if (!sound_LevelUp.sound.loadFromFile(path))
        {
            std::cerr << "Unable to load sound from path: " << path << std::endl;
        }
        sound_LevelUp.bLoaded = true;
    }

    return sound_LevelUp.sound;
}

const sf::SoundBuffer& SoundManager::getSound_FlashlightToggle1()
{
    static const std::string path = "Content/Sounds/Flashlight_Toggle1.wav";
    
    if (!sound_flashl_toggle1.bLoaded)
    {
        if (!sound_flashl_toggle1.sound.loadFromFile(path))
        {
            std::cerr << "Unable to load sound from path: " << path << std::endl;
        }
        sound_flashl_toggle1.bLoaded = true;
    }

    return sound_flashl_toggle1.sound;
}

const sf::SoundBuffer& SoundManager::getSound_FlashlightToggle2()
{
    static const std::string path = "Content/Sounds/Flashlight_Toggle2.wav";
    
    if (!sound_flashl_toggle2.bLoaded)
    {
        if (!sound_flashl_toggle2.sound.loadFromFile(path))
        {
            std::cerr << "Unable to load sound from path: " << path << std::endl;
        }
        sound_flashl_toggle2.bLoaded = true;
    }

    return sound_flashl_toggle2.sound;
}
