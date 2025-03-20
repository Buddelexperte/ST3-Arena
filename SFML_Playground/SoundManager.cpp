#pragma once
#include <iostream>
#include <memory>

#include "SoundManager.h"


void SoundManager::play(const sf::SoundBuffer& buffer)
{
    std::unique_ptr<sf::Sound> newSound = std::make_unique<sf::Sound>(buffer); // Create new Sound Object
    newSound->play(); // Play the sound immediately
    activeSounds.push_back(std::move(newSound));
}

void SoundManager::cleanUp()
{
    // Remove all sounds that are not playing anymore
    activeSounds.erase(
        std::remove_if(activeSounds.begin(), activeSounds.end(),
            [](const std::unique_ptr<sf::Sound>& sound) {
                return sound->getStatus() == sf::Sound::Stopped;
            }),
        activeSounds.end());
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