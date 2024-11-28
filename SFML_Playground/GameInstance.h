#pragma once
#include <iostream>
#include "Functions.h"

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

// Forward Declarations
class InputWidget;
class Player;


// Global Game Instance ---------------------------------------------------------------------------


class GI_Arena // SINGLETON PATTERN
{
private:
	sf::Clock clock;
	float deltaTime = 0.0f;
	float fps = 0.0f;

	GI_Arena();
	GI_Arena(const GI_Arena&) = delete;
	GI_Arena& operator=(const GI_Arena&) = delete; // Block the '=' operator to stop copies being made of this class

	sf::RenderWindow* window = nullptr;
	sf::View* view = nullptr;
	sf::RenderStates states;

	E_GameState gameState = MENU_SCREEN;
	// Create all main widgets for later use
	std::vector<std::unique_ptr<InputWidget>> widgets;

	float zoomFactor = 1.0f;
	InputWidget* activeMenu = nullptr;
	Player* playerRef = nullptr;

	bool bIsGameplayPaused = true;
public:
	static GI_Arena& getInstance()
	{
		static GI_Arena instance;
		return instance;
	}
	// Tick and such
	bool initWidgets();
	void start();
	void preTick();
	void tick(const float&);
	void updateScreen();
	void setViewCenter(const sf::Vector2f&);
	// Important values and references
	sf::RenderWindow* getWindow() const { return window; }
	sf::View* getView() const { return view; }
	sf::RenderStates getRenderStates() const { return states; }
	Player* getPlayer();
	void setZoom(const float& newZoom) { view->zoom(zoomFactor = newZoom); }
	float getZoom() const { return zoomFactor; }
	InputWidget* getActiveWidget() { return activeMenu; }
	void setIsPaused(const bool& bPause) { bIsGameplayPaused = bPause; }
	bool getIsPaused() const { return bIsGameplayPaused; }
	// Input stuff
	bool handleEvent(sf::Event*);
	sf::Vector2f getMousePos() { return window->mapPixelToCoords(sf::Mouse::getPosition(*window)); }
	
	void setGameState(const E_GameState&);
	E_GameState getGameState() const { return gameState; }
};