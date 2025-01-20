#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class FontManager
{
private:
    FontManager()
    {
        loadFonts();
    }

    ~FontManager() = default;

    std::vector<std::unique_ptr<sf::Font>> allFonts;

    // Verbiete Kopieren und Zuweisung
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;

public:
    void loadFonts()
    {
        static bool didOnce = false;

        if (didOnce) return;
        didOnce = true;

        const std::vector<std::string> fontPaths = {
            "Content/fonts/coolvetica/coolvetica_rg.otf" // [0]
        };

        for (const std::string& path : fontPaths)
        {
            std::unique_ptr<sf::Font> newFont = std::make_unique<sf::Font>();
            if (!newFont->loadFromFile(path))
                std::cerr << "Unable to create font from path: " << path << std::endl;
            else
                allFonts.push_back(std::move(newFont));
        }

    }
    static FontManager& getInstance()
    {
        static FontManager instance;
        return instance;
    }

    const sf::Font& getFont(const int& index)
    {
        if (index < 0 || index >= allFonts.size())
        {
            throw std::out_of_range("Font index out of range");
        }
        return *allFonts[index];
    }
};