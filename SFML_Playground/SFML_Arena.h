#pragma once
#include "Button.h"
#include "Timer.h"
#include "TargetController.h" // TargetSpawner and Handler
#include <fstream> // File operations
#include <iostream> // Console Log and File manipulation
#include <vector> // Shape storage and access
#include <string>

const std::string SAVE_FILE = "../SaveGame.txt";

class WidgetMenu;

class SaveGame
{
private:
public:
	static int Stored_Save;
	static int loadSavedData(const std::string& path = SAVE_FILE)
	{
		std::ifstream inFile(path);  // Open file in input mode and write the highscore to it
		if (inFile.is_open()) {
			inFile >> SaveGame::Stored_Save;
			inFile.close();
			std::cout << "SaveData loaded!\n";
		}
		else {
			std::cerr << "Error opening save file for reading. Defaulting to 0.\n"; // Display file access error message
		}
		return SaveGame::Stored_Save;
	}
	static void saveData()
	{
		std::ofstream outFile(SAVE_FILE); // Open file in output mode and write the highscore to it
		if (outFile.is_open()) {
			outFile << SaveGame::Stored_Save;
			outFile.close();
			std::cout << "SaveData saved!\n";
			std::cout << SaveGame::Stored_Save << std::endl;
		}
		else {
			std::cerr << "Error opening save file for writing.\n"; // Display file access error message
		}
	}
};

// Enum for handling gameStages
enum E_GameState {
	GAME_ENDED = -1, // Not started or interrupted
	MENU_SCREEN = 0, // A Menu with clickable buttons
	PAUSED,
	GAME_OVER,
	GAME_LAUNCHING, // gameLoop should start and execute init functionality
	IN_GAME // gameLoop should start
};

// Global Game Instance
class GI_Arena // SINGLETON PATTERN
{
private:
	GI_Arena();
	GI_Arena(const GI_Arena&) = delete;
	GI_Arena& operator=(const GI_Arena&) = delete;

	sf::RenderWindow* window = nullptr;
	sf::RenderStates states;
	sf::Vector2u windowSize;
	sf::Vector2f windowCenter;

	E_GameState gameState = GAME_ENDED;
public:
	static GI_Arena& getInstance()
	{
		static GI_Arena instance;
		return instance;
	}

	void update();
	void draw(sf::Drawable*);
	sf::RenderWindow* getWindow() { return window; }
	sf::RenderStates getRenderStates() { return states; }

	E_GameState getGameState() const
	{
		return gameState;
	}
	void setGameState(const E_GameState&);
	bool getGameStateChanges(E_GameState& oldGS) const 
	{
		if (oldGS != gameState)
		{
			oldGS = gameState;
			return true;
		}
		return false;
	}
};

class WidgetMenu : public sf::Drawable
{
protected:
	GI_Arena& gameInstance = GI_Arena::getInstance();
	sf::RenderTarget* window = gameInstance.getWindow();
	sf::Vector2u windowSize;
	sf::Vector2f windowCenter;
	std::vector<sf::Drawable*> shapes;
public:
	WidgetMenu()
	{
		windowUpdate();
	}
	virtual void windowUpdate()
	{
		windowSize = window->getSize();
		windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
	}
	virtual void init() {};
	virtual void update(const float& deltaTime) { windowUpdate(); };
	virtual bool isInteracted(const sf::Vector2f& mousePos) { return false; };
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (const auto& elem : shapes) target.draw(*elem, states);
	}
};

class W_MainMenu : public WidgetMenu
{
private:
	Button* menu_title;
	Button* menu_highscore;
	Button* menu_startButton;
	Button* menu_optionsButton;
	Button* menu_quitButton;
public:
	W_MainMenu();
	void init() override { menu_highscore->setText("Highscore: " + std::to_string(SaveGame::Stored_Save)); }
	bool isInteracted(const sf::Vector2f&) override;
};

class W_Gameplay : public WidgetMenu
{
private:
	const float startTimer = 10.0f;
	const float minTimer = 1.0f;
	int hitTargets = 0;
	Timer* healthBar;
	TargetController* targetController;
public:
	W_Gameplay();
	void init() override;
	void update(const float& deltaTime) override;
	bool isInteracted(const sf::Vector2f& mousePos);

};