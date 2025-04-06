#pragma once
#include <SFML/Audio.hpp>

class SoundManager
{
private:
    static const inline std::string SOUND_DIR = "Content/Sounds/";

    SoundManager();
    ~SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete; // Block the '=' operator to stop copies being made of this class

    std::vector<std::unique_ptr<sf::Sound>> activeSounds;
    
    struct LoadedSound
    {
        sf::SoundBuffer sound;
        bool bLoaded = false;
    };

    LoadedSound sound_click;
    LoadedSound sound_returnClick;

public:
    static SoundManager& getInstance()
    {
        static SoundManager instance;
        return instance;
    }

    const sf::SoundBuffer& getSound_Click();
    const sf::SoundBuffer& getSound_ReturnClick();

    void play(const sf::SoundBuffer& buffer);
    void cleanUp();
};