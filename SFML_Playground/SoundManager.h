#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>

class SoundManager
{
private:
    SoundManager() = default;
    ~SoundManager() = default;

    sf::SoundBuffer soundBuffer_click;
    sf::SoundBuffer soundBuffer_returnClick;

    bool clickBufferLoaded = false;
    bool returnClickBufferLoaded = false;

    // Verbiete Kopieren und Zuweisung
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
public:
    static SoundManager& getInstance()
    {
        static SoundManager instance;
        return instance;
    }

    const sf::SoundBuffer& getClickBuffer()
    {
        if (!clickBufferLoaded)
        {
            if (!soundBuffer_click.loadFromFile("Content/Sounds/Glitch.wav"))
                std::cerr << "Unable to load click sound!" << std::endl;
            clickBufferLoaded = true;
        }
        return soundBuffer_click;
    }

    const sf::SoundBuffer& getReturnClickBuffer()
    {
        if (!returnClickBufferLoaded)
        {
            if (!soundBuffer_returnClick.loadFromFile("Content/Sounds/Glitch2.wav"))
                std::cerr << "Unable to load click return sound!" << std::endl;
            returnClickBufferLoaded = true;
        }
        return soundBuffer_returnClick;
    }

};