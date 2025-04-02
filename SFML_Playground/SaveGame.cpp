#pragma once

#include "SaveGame.h"

const std::string SAVE_FILE = "SaveGame.txt";

// SaveGame Code ----------------------------------------------------------------------------------

SaveGame_Struct SaveGame::currentData;
SaveGame_Struct SaveGame::storedData;

SaveGame_Struct SaveGame::loadSavedData(const std::string& path)
{
	std::ifstream inFile(path);  // Open file in input mode and write the highscore to it
	if (inFile.is_open()) {
		inFile >> storedData.enemiesKilled;
		inFile >> storedData.weaponName;
		inFile.close();
		std::cout << "SaveData loaded!\n";
	}
	else {
		std::cerr << "Error opening save file for reading. Defaulting to 0.\n"; // Display file access error message
	}
	return storedData;
}

void SaveGame::saveData()
{
	storedData = currentData;
	std::ofstream outFile(SAVE_FILE); // Open file in output mode and write the highscore to it
	if (outFile.is_open()) {
		outFile << storedData.enemiesKilled << '\n';
		outFile << storedData.weaponName << '\n';
		outFile.close();
		std::cout << "SaveData saved! Score [" << std::to_string(storedData.enemiesKilled) << "]\n";
	}
	else {
		std::cerr << "Error opening save file for writing.\n"; // Display file access error message
	}
}
