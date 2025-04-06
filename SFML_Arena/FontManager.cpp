#pragma once
#include <iostream>

#include "FontManager.h"

FontManager::FontManager()
{
    loadFonts();
}

void FontManager::loadFonts()
{
    static bool didOnce = false;

    if (didOnce) return;
    didOnce = true;

    static const std::vector<std::string> fontPaths = {
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

const sf::Font& FontManager::getFont(const int& index)
{
    if (index < 0 || index >= allFonts.size())
    {
        throw std::out_of_range("Font index out of range");
    }
    return *allFonts[index];
}

const sf::Font& FontManager::getFont(const Font& EFont)
{
    return getFont(static_cast<int>(EFont));
}