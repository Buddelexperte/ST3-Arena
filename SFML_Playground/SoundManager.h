#pragma once
#include <SFML/Audio.hpp>

class SoundManager
{
private:
    static const inline std::string SOUND_DIR = "Content/Sounds/";

    struct LoadedSound
    {
        sf::SoundBuffer sound;
        bool bLoaded = false;
    };

    SoundManager() = default;
    ~SoundManager() = default;

    std::vector<std::unique_ptr<sf::Sound>> activeSounds;

    LoadedSound sound_click;
    LoadedSound sound_returnClick;

    // Verbiete Kopieren und Zuweisung
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

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