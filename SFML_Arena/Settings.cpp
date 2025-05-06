#pragma once

#include "Settings.h" // Own header file
#include "GameInstance.h"

UserSettings_Struct UserSettings::settings;

UserSettings_Struct UserSettings::loadSettings(const std::string& path)
{
	std::ifstream inFile(path);  // Open file in input mode and write the highscore to it
	if (inFile.is_open()) {
		inFile >> settings.maxFPS;
		inFile >> settings.bUseVSync;
		inFile >> settings.bFullscreen;
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

	gameInstance().applySettings(settings);

	std::ofstream outFile(SETTINGS_FILE); // Open file in output mode and write the highscore to it
	if (outFile.is_open())
	{
		outFile << settings.maxFPS << '\n';
		outFile << settings.bUseVSync << '\n';
		outFile << settings.bFullscreen << '\n';
		outFile << settings.bWidgetParallax << '\n';
		outFile.close();
		std::cout << "Settings saved!\n";
	}
	else {
		std::cerr << "Error opening settings file for writing.\n"; // Display file access error message
	}
}
