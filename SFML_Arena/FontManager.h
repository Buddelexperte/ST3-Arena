#pragma once
#include <SFML/Graphics.hpp>

class FontManager
{
private:
    FontManager();
    ~FontManager() = default;
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete; 

    std::vector<std::unique_ptr<sf::Font>> allFonts;

public:
    static FontManager& getInstance()
    {
        static FontManager instance;
        return instance;
    }

    enum class Font
    {
        NO_FONT = -1,
        BUTTON_FONT = 0
    };

    void loadFonts();

    const sf::Font& getFont(const int& index);
    const sf::Font& getFont(const Font& EFont);
};