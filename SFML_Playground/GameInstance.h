#pragma once
#include <iostream>
#include "BaseClasses.h"
#include "Functions.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "EnemyManager.h"
#include "RandomNumbers.h"

// Enum for handling gameStages in GameInstance
enum E_GameState {
	QUIT = -1, // Not yet started or game has been interrupted
	MENU_SCREEN = 0, // A Menu with clickable buttons
	UNPAUSED, // From pause to not paused
	GAME_PAUSED, // Freeze gamepay and show PauseMenu
	GAME_OVER, // Game ended somehow, freezes and show GameOverMenu
	GAME_LAUNCHING, // gameLoop should start and execute init functionality
	IN_GAME // gameLoop should start
};


// Global Game Instance ---------------------------------------------------------------------------


class GI_Arena // SINGLETON PATTERN
{
private:
	// Time calculation (deltaTime etc.)
	sf::Clock clock;

	// Privatize constructors and copy operators
	GI_Arena();
	GI_Arena(const GI_Arena&) = delete;
	GI_Arena& operator=(const GI_Arena&) = delete; // Block the '=' operator to stop copies being made of this class
	// Manager instances
	SoundManager& soundManager = SoundManager::getInstance();
	FontManager& fontManager = FontManager::getInstance();
	EnemyManager& enemyManager = EnemyManager::getInstance();
	// SFML Viewport objects
	sf::RenderWindow* window = nullptr;
	sf::View* view = nullptr;
	sf::RenderStates states;
	// Custom GameState
	E_GameState gameState = MENU_SCREEN;

	// Widget variables
	std::vector<std::shared_ptr<InputWidget>> widgets;
	std::shared_ptr<InputWidget> activeMenu = nullptr;

	Player* player = nullptr;
	// Somehow Gameplay related?
	sf::Vector2f prevCamPos;
	float zoomFactor = 1.0f;
	bool bIsGameplayPaused = true;
public:
	static GI_Arena& getInstance() // SINGLETON getInstance
	{
		static GI_Arena instance;
		return instance;
	}
	// Basics
	void start(); // Function to call from main()
	bool initWidgets();
	bool makePlayer();
	void correctWidget();
	// Ticks
	void preTick(); // Logic before Player input (Checks for Inventory etc.)
	void tick(const float&); // Player Input and window.draw()
	void postTick(); // Logic after Player Input (Checks for game closing etc.)
	// Viewport
	void updateScreen();
	void tickView(const float&);
	void resetViewPos();
	void setViewPos(const sf::Vector2f&);
	// Setters and Getters of pointers and important variables
	sf::RenderWindow* getWindow() const { return window; }
	sf::View* getView() const { return view; }
	sf::RenderStates getRenderStates() const { return states; }
	Player* getPlayer();
	std::weak_ptr<InputWidget> getActiveWidget() { return activeMenu; }
	void setIsPaused(const bool& bPause) { bIsGameplayPaused = bPause; }
	bool getIsPaused() const { return bIsGameplayPaused; }
	// Update Zoom (WIP)
	void setZoom(const float& newZoom) { view->zoom(zoomFactor = newZoom); }
	float getZoom() const { return zoomFactor; }
	// Input stuff
	bool handleEvent(sf::Event*);
	sf::Vector2f getMousePos() { return window->mapPixelToCoords(sf::Mouse::getPosition(*window)); }
	// GameState 
	void setGameState(const E_GameState&);
	E_GameState getGameState() const { return gameState; }
};