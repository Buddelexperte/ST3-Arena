#pragma once

#include "TextureManager.h"
#include <iostream>

TextureManager::TextureManager()
{
	static const std::string dir = "Content/Textures";

	static const std::unordered_map<ETexture, std::string> paths = {
		{ETexture::BUTTON,					"/UI/ButtonTexture.png"},
		{ETexture::BUTTON_SQR,				"/UI/SquareTexture.png"},
		{ETexture::PERK_OFF,				"/Icons/offense_perk.png"},
		{ETexture::PERK_DEF,				"/Icons/defense_perk.png"},
		{ETexture::PERK_UTIL,				"/Icons/utility_perk.png"},
		{ETexture::PERK_SUP,				"/Icons/support_perk.png"},
		{ETexture::OPTIONS_GRAPHICS_BORDER,	"/UI/Options_Graphics_BorderTexture.png"},
		{ETexture::LEVEL_UP_BORDER,			"/UI/Level_Up_Border.png"},
		{ETexture::PERK_FAMILY_BORDER,		"/UI/Perk_Family_Border.png"},
	};

	// Loop over textures and try to load each one
	for (const auto& [id_enum, tex] : textures)
	{
		auto path_it = paths.find(id_enum);
		if (path_it == paths.end())
		{
			std::cerr << "No path found for texture ID" << static_cast<int>(id_enum) << std::endl;
			notLoaded.insert(id_enum);
			continue;
		}

		const std::string& path = dir + path_it->second;
		if (!tex->loadFromFile(path))
		{
			std::cerr << "Failed to load texture: " << path << std::endl;
			notLoaded.insert(id_enum);
			continue;
		}
	}
}

sf::Texture* TextureManager::getTexturePtr(const ETexture id)
{
	auto it = textures.find(id);
	if (it == textures.end())
	{
		std::cout << "ETexture not valid!" << std::endl;
		return nullptr;
	}

	return it->second;
}
