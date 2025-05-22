#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <unordered_map>

enum class ETexture
{
    NONE = -1,
    BUTTON = 0,
    BUTTON_SQR,
    PERK_OFF,
    PERK_DEF,
    PERK_UTIL,
    PERK_SUP,
    OPTIONS_GRAPHICS_BORDER,
    LEVEL_UP_BORDER,
    PERK_FAMILY_BORDER
};

class TextureManager
{
private:

    static const inline std::string SOUND_DIR = "Content/Sounds/";

    TextureManager();
    ~TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete; // Block the '=' operator to stop copies being made of this class

    sf::Texture Tex_button;
    sf::Texture Tex_button_sqr;
    sf::Texture Tex_perk_off;
    sf::Texture Tex_perk_def;
    sf::Texture Tex_perk_util;
    sf::Texture Tex_perk_sup;
    sf::Texture Tex_optionsGraphicsBorder;
    sf::Texture Tex_levelUpBorder;
    sf::Texture Tex_perkFamilyBorder;

    std::unordered_set<ETexture> notLoaded;
    const std::unordered_map<ETexture, sf::Texture*> textures = {
        {ETexture::BUTTON,                  &Tex_button},
        {ETexture::BUTTON_SQR,              &Tex_button_sqr},
        {ETexture::PERK_OFF,                &Tex_perk_off},
        {ETexture::PERK_DEF,                &Tex_perk_def},
        {ETexture::PERK_UTIL,               &Tex_perk_util},
        {ETexture::PERK_SUP,                &Tex_perk_sup},
        {ETexture::OPTIONS_GRAPHICS_BORDER, &Tex_optionsGraphicsBorder},
        {ETexture::LEVEL_UP_BORDER,         &Tex_levelUpBorder },
        {ETexture::PERK_FAMILY_BORDER,      &Tex_perkFamilyBorder }
    };

public:
    static TextureManager& getInstance()
    {
        static TextureManager instance;
        return instance;
    }

    sf::Texture* getTexturePtr(const ETexture id);
};