#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

// SAVE GAME --------------------------------------------------------------------------------------

struct UserSettings_Struct
{
	unsigned int maxFPS		= 144;
	bool bUseVSync			= true;
	bool bFullscreen		= true;
	sf::Vector2u res		= sf::Vector2u(1920, 1080);
	bool bWidgetParallax	= false;

	void clear()
	{
		maxFPS				= 144;
		bUseVSync			= true;
		bFullscreen			= true;
		res					= sf::Vector2u(1920, 1080);
		bWidgetParallax		= false;
	}
};

class UserSettings
{
private:
	static const inline std::string SETTINGS_FILE = "Settings.txt";
public:
	static UserSettings_Struct settings;

	static UserSettings_Struct loadSettings(const std::string & = SETTINGS_FILE);
	static void saveSettings(UserSettings_Struct settingsToSave);
};