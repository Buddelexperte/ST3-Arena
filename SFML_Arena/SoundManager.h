#pragma once
#include <SFML/Audio.hpp>

enum class ESoundEnv
{
    MASTER = 0,
    UI,
    GAMEPLAY,
    MUSIC
};

class SoundManager
{
private:

    static const inline std::string SOUND_DIR = "Content/Sounds/";

    SoundManager();
    ~SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete; // Block the '=' operator to stop copies being made of this class

    struct SoundEnvironment
    {
        std::unique_ptr<sf::Sound> sound;
        ESoundEnv environment;

        SoundEnvironment(std::unique_ptr<sf::Sound> s, const ESoundEnv env)
            : sound(std::move(s))
        {
            environment = env;
        }
    };

    std::vector<SoundEnvironment> activeSounds_master; // Owns the unique ptrs
    float volume_master = 100.0f; // Sound an
    bool bMuted_master = false;

    bool bMuted_ui = false;
    bool bMuted_gameplay = false;
    bool bMuted_music = false;

    float volume_ui_f = 1.0f; 
    float volume_gameplay_f = 1.0f;
    float volume_music_f = 1.0f; 
    float volume_music_f_paused = volume_music_f * 0.7f;

    struct LoadedSound
    {
        sf::SoundBuffer sound;
        bool bLoaded = false;
    };

    bool gameplaySoundsPaused = false;

    LoadedSound sound_click;
    LoadedSound sound_returnClick;

    sf::Music backgroundMusic;

public:
    static SoundManager& getInstance()
    {
        static SoundManager instance;
        return instance;
    }

    const sf::SoundBuffer& getSound_Click();
    const sf::SoundBuffer& getSound_ReturnClick();

    void play(const sf::SoundBuffer& buffer, const ESoundEnv);
    void cleanUp();

    void playMusic(const std::string& filePath);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();

    void pauseGameplaySound();
    void resumeGameplaySound();

    float getMasterVolume() const;
    void setMasterVolume(const float);

    float getVolumeFactor(const ESoundEnv) const;
    void setVolumeFactor(const float, const ESoundEnv);

    bool getIsMuted(const ESoundEnv = ESoundEnv::MASTER) const;
    void setIsMuted(const bool, const ESoundEnv = ESoundEnv::MASTER);

    float getActualVolume(const ESoundEnv = ESoundEnv::MASTER) const;
    void updateOnSoundSettings();
};