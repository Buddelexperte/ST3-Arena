#pragma once
#include <fstream>
#include <string>
#include <iostream>

// SAVE GAME --------------------------------------------------------------------------------------

extern const std::string SAVE_FILE;

struct SaveGame_Struct
{
	int score;
	std::string weaponName;

	SaveGame_Struct(int score = 0, std::string weaponName = "Keine Waffe")
		: score(score), weaponName(weaponName)
	{}

	void clear()
	{
		score = 0;
		weaponName = "Keine Waffe";
	}
};

class SaveGame
{
public:
	static SaveGame_Struct currentData;
	static SaveGame_Struct storedData;

	static SaveGame_Struct loadSavedData(const std::string& = SAVE_FILE);
	static void saveData();
};