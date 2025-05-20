
#pragma once

#include "SaveGame.h" // Own header file

// SaveGame Code ----------------------------------------------------------------------------------

SaveGame_Struct SaveGame::currentData;
SaveGame_Struct SaveGame::storedData;
Highscore_Struct SaveGame::currentHighscore;

Highscore_Struct SaveGame::getCurrHighscore()
{
	return currentHighscore;
}

void SaveGame::updateHighscore()
{
	// Update max_kills
	if (currentData.enemiesKilled > currentHighscore.max_kills)
		currentHighscore.max_kills = currentData.enemiesKilled;

	// Update highscore
	if (currentData.score > currentHighscore.highscore)
		currentHighscore.highscore = currentData.score;

	// Update max_secondsPlayed
	if (currentData.secondsPlayed > currentHighscore.max_secondsSurvived)
		currentHighscore.max_secondsSurvived = currentData.secondsPlayed;

	std::ofstream outFile(HIGHSCORE_FILE); // Open file in output mode and write the highscore to it
	if (outFile.is_open()) {
		outFile << currentHighscore.max_kills << '\n';
		outFile << currentHighscore.highscore << '\n';
		outFile << currentHighscore.max_secondsSurvived << '\n';
		outFile.close();
		std::cout << "New Highscore saved! Score [" << std::to_string(currentHighscore.highscore) << "]\n";
	}
	else {
		std::cerr << "Error opening save file for writing.\n"; // Display file access error message
	}
}

void SaveGame::loadHighscores()
{
	std::ifstream inFile(HIGHSCORE_FILE);  // Open file in input mode and write the highscore to it
	if (inFile.is_open()) {
		inFile >> currentHighscore.max_kills;
		inFile >> currentHighscore.highscore;
		inFile >> currentHighscore.max_secondsSurvived;
		inFile.close();
		std::cout << "Highscores loaded!\n";
	}
	else {
		std::cerr << "Error opening save file for reading. Defaulting to 0.\n"; // Display file access error message
	}
}

SaveGame_Struct SaveGame::loadSavedData()
{
	loadHighscores();

	std::ifstream inFile(SAVE_FILE);  // Open file in input mode and write the highscore to it
	if (inFile.is_open()) {
		inFile >> storedData.enemiesKilled;
		inFile >> storedData.score;
		inFile >> storedData.weaponName;
		inFile >> storedData.secondsPlayed;
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
	updateHighscore();

	storedData = currentData;

	std::ofstream outFile(SAVE_FILE); // Open file in output mode and write the highscore to it
	if (outFile.is_open()) {
		outFile << storedData.enemiesKilled << '\n';
		outFile << storedData.score << '\n';
		outFile << storedData.weaponName << '\n';
		outFile << storedData.secondsPlayed << '\n';
		outFile.close();
		std::cout << "SaveData saved! Score [" << std::to_string(storedData.score) << "]\n";
	}
	else {
		std::cerr << "Error opening save file for writing.\n"; // Display file access error message
	}
}
