#pragma once

#include "Settings.h" // Own header file
#include "GameInstance.h"

UserSettings_Struct UserSettings::settings;

std::vector<ResolutionDesc>& UserSettings::getResolutions()
{
	return availableResolutions;
}

size_t UserSettings::getResolutionIndex(const sf::Vector2u& targetRes)
{
	for (size_t i = 0; i < availableResolutions.size(); ++i)
	{
		// Compare the resolutions
		if (availableResolutions[i].res == targetRes)
		{
			return i; // Return the index when a match is found
		}
	}

	std::cerr << "Resolution not found! Resolution: " << targetRes.x << "x" << targetRes.y << std::endl;
	return static_cast<size_t>(0); // Return first index, smalles resolution, should be possible anywhere
}

UserSettings_Struct UserSettings::loadSettings(const std::string& path)
{
	std::ifstream inFile(path);  // Open file in input mode and write the highscore to it
	if (inFile.is_open()) {
		inFile >> settings.maxFPS;
		inFile >> settings.bUseVSync;
		inFile >> settings.bFullscreen;
		inFile >> settings.resID;
		inFile >> settings.bWidgetParallax;
		inFile.close();
		std::cout << "Settings loaded!\n";
	}
	else {
		std::cerr << "Error opening settings file for reading. Defaulting to 0.\n"; // Display file access error message
	}
	return settings;
}

void UserSettings::saveSettings(UserSettings_Struct settingsToSave)
{
	settings = settingsToSave;

	std::ofstream outFile(SETTINGS_FILE); // Open file in output mode and write the highscore to it
	if (outFile.is_open())
	{
		outFile << settings.maxFPS << '\n';
		outFile << settings.bUseVSync << '\n';
		outFile << settings.bFullscreen << '\n';
		outFile << settings.resID << '\n';
		outFile << settings.bWidgetParallax << '\n';
		outFile.close();
		std::cout << "Settings saved!\n";
	}
	else {
		std::cerr << "Error opening settings file for writing.\n"; // Display file access error message
	}
}

void UserSettings::saveSettings()
{
	saveSettings(UserSettings::settings);
}
