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
	size_t resID			= 0;
	bool bWidgetParallax	= false;

	void clear()
	{
		maxFPS				= 144;
		bUseVSync			= true;
		bFullscreen			= true;
		resID				= 0;
		bWidgetParallax		= false;
	}

	bool operator==(const UserSettings_Struct& other) const
	{
		return maxFPS == other.maxFPS &&
			bUseVSync == other.bUseVSync &&
			bFullscreen == other.bFullscreen &&
			resID == other.resID &&
			bWidgetParallax == other.bWidgetParallax;
	}

	bool operator!=(const UserSettings_Struct& other) const
	{
		return !(*this == other);
	}
};

struct ResolutionDesc
{
	sf::Vector2u xy;
	std::string desc;
};

class UserSettings
{
private:
	// --- RESOLUTION ---
	static inline std::vector<ResolutionDesc> availableResolutions = {
	{sf::Vector2u(0, 0), "(Native)"},					// Native
	{sf::Vector2u(640, 480), "(VGA) 640x480"},				// VGA
	{sf::Vector2u(800, 600), "(SVGA) 800x600"},				// SVGA
	{sf::Vector2u(1024, 768), "(XGA) 1024x768"},			// XGA
	{sf::Vector2u(1280, 720), "(HD) 1280x720"},				// HD
	{sf::Vector2u(1366, 768), "(WXGA) 1366x768"},			// WXGA
	{sf::Vector2u(1440, 900), "(WXGA+) 1440x900"},			// WXGA+
	{sf::Vector2u(1600, 900), "(HD+) 1600x900"},			// HD+
	{sf::Vector2u(1680, 1050), "(WSXGA+) 1680x1050"},		// WSXGA+
	{sf::Vector2u(1600, 1200), "(UXGA) 1600x1200"},			// UXGA
	{sf::Vector2u(1920, 1080), "(Full HD) 1920x1080"},		// Full HD
	{sf::Vector2u(1920, 1200), "(WUXGA) 1920x1200"},		// WUXGA
	{sf::Vector2u(2560, 1440), "(QHD) 2560x1440"},			// QHD
	{sf::Vector2u(2560, 1600), "(WQXGA) 2560x1600"},		// WQXGA
	{sf::Vector2u(3440, 1440), "(UWQHD) 3440x1440"},		// UWQHD
	{sf::Vector2u(3840, 2160), "(4K UHD) 3840x2160"},		// 4K UHD
	{sf::Vector2u(5120, 2880), "(5K) 5120x2880"},			// 5K
	{sf::Vector2u(7680, 4320), "(8K UHD) 7680x4320"}		// 8K UHD
	};

	static const inline std::string SETTINGS_FILE = "Settings.txt";
	static std::vector<ResolutionDesc>& getResolutionsVec();
	static UserSettings_Struct settings;

public:
	static UserSettings_Struct getSettings();
	static size_t getNumResolutions();
	static sf::VideoMode getResolution(const size_t);
	static std::string getResolutionDesc(const size_t);

	static size_t getResolutionIndex(const sf::Vector2u& targetRes);

	static UserSettings_Struct loadSettings(const std::string & = SETTINGS_FILE);
	static void saveSettings(UserSettings_Struct settingsToSave);
	static void saveSettings();
};