#pragma once
#include <fstream>
#include <string>
#include <iostream>

// SAVE GAME --------------------------------------------------------------------------------------


struct SaveGame_Struct
{
	int enemiesKilled;
	int score;
	std::string weaponName;
	int secondsPlayed;

	SaveGame_Struct(int kills = 0, int score = 0, std::string weaponName = "Keine Waffe", int secs = 0)
		: enemiesKilled(kills), score(score), weaponName(weaponName), secondsPlayed(secs)
	{}

	void clear()
	{
		enemiesKilled = 0;
		score = 0;
		weaponName = "Keine Waffe";
		secondsPlayed = 0;
	}
};

struct Highscore_Struct
{
	int max_kills;
	int highscore;
	int max_secondsSurvived;

	Highscore_Struct(int mkills = 0, int hs = 0, int msplayed = 0)
		: max_kills(mkills), highscore(hs), max_secondsSurvived(msplayed)
	{}

	void clear()
	{
		max_kills = 0;
		highscore = 0;
		max_secondsSurvived = 0;
	}
};

class SaveGame
{
private:
	static const inline std::string SAVE_FILE = "SaveGame.txt";
	static const inline std::string HIGHSCORE_FILE = "HsGame.txt";

	static Highscore_Struct currentHighscore;
	static void updateHighscore();
	static void loadHighscores();

public:
	static SaveGame_Struct currentData;
	static SaveGame_Struct storedData;

	static SaveGame_Struct loadSavedData();
	static void saveData();

	static Highscore_Struct getCurrHighscore();
};