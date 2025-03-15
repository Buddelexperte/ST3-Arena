#pragma once
#include <iostream>

#include "Player.h"
#include "Functions.h"
#include "RandomNumbers.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "CollisionManager.h"
#include "SaveGame.h"

// Global Game Instance ---------------------------------------------------------------------------


class GI_Arena // SINGLETON PATTERN
{
private:
	// Time calculation (deltaTime etc.)
	sf::Clock clock;

	// SINGELTON
	GI_Arena();
	GI_Arena(const GI_Arena&) = delete;
	GI_Arena& operator=(const GI_Arena&) = delete; // Block the '=' operator to stop copies being made of this class
	
	// Manager instances
	SoundManager& soundManager = SoundManager::getInstance();
	FontManager& fontManager = FontManager::getInstance();
	CollisionManager& collisionManager = CollisionManager::getInstance();
	// SFML Viewport objects
	sf::RenderWindow* window = nullptr;
	sf::View* view = nullptr;
	sf::RenderStates states;
	// Custom GameState
	GameState gameState = MENU_SCREEN;

	// Widget variables
	std::vector<std::shared_ptr<InputWidget>> widgets;
	std::shared_ptr<InputWidget> activeMenu = nullptr;

	Player* player = nullptr;
	Player* makePlayer();
	// Somehow Gameplay related?
	sf::Vector2f prevCamPos;
	float zoomFactor = 1.0f;
	bool bIsGameplayPaused = true;

public:
	static GI_Arena& getInstance() // SINGLETON getInstance()
	{
		static GI_Arena instance;
		return instance;
	}
	// Basics
	void start(); // Function to call from main()
	bool initWidgets();
	void correctWidget();

	void launchGame();
	void startRound();
	// Ticks
	void tickLoop();
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
	void setGameState(const GameState&);
	GameState getGameState() const { return gameState; }
};