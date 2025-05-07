#pragma once
#include <SFML/Graphics.hpp>

#include "BaseTypes.h"
#include "MainMenu.h"
#include "Gameplay.h"
#include "Player.h"

#include "Settings.h"

// Global Game Instance ---------------------------------------------------------------------------

class GI_Arena // SINGLETON PATTERN
{
private:
	// Debugging mode
	static constexpr bool bDevMode = true;
	
	// CONFIG
	UserSettings_Struct usedSettings;

	// Time calculation (deltaTime etc.)
	sf::Clock clock;
	static float globalTime;


	// SINGLETON
	GI_Arena();
	GI_Arena(const GI_Arena&) = delete;
	GI_Arena& operator=(const GI_Arena&) = delete; // Block the '=' operator to stop copies being made of this class
	
	// SFML Viewport objects
	static const inline std::string WINDOW_NAME = "ARENA";
	std::unique_ptr<sf::RenderWindow> window = nullptr;
	sf::Image window_icon;
	std::unique_ptr<sf::View> view = nullptr;
	sf::RenderStates states;

	void createViewport(); // For init

	// Custom GameState
	GameState gameState = LOADING_SCREEN;

	// Widget variables
	std::vector<std::shared_ptr<InputWidget>> widgets;
	std::shared_ptr<InputWidget> activeMenu = nullptr;
	bool bIsGameplayPaused = true;

	// Player
	Player* validPlayer();
	std::unique_ptr<Player> player = nullptr;

	// View related
	sf::Vector2f prevCamPos;
	float zoomFactor = 1.0f;
	sf::Vector2f widgetOffset;
	
	void preTick(); // Logic before Player input (Checks for Inventory etc.)
	void tick_view(const float&);
	void updateWidgetOffset(const sf::Vector2f& mouseOffset);

	void tick(const float&); // Player Input and window.draw()
	void postTick(); // Logic after Player Input (Checks for game closing etc.)

	void correctWidget();

public:
	static GI_Arena& getInstance() // SINGLETON getInstance()
	{
		static GI_Arena instance;
		return instance;
	}

	// Basics
	void start(); // Function to call from main()
	bool initWidgets();

	void launchGame();
	void startRound();
	// Ticks*
	void tickLoop();
	float getGlobalTime() const
	{
		return globalTime;
	}
	// Viewport
	void updateScreen();
	void resetViewPos();
	void setViewPos(const sf::Vector2f&);
	// Setters and Getters of pointers and important variables
	sf::RenderWindow* getWindow() const { return window.get(); }
	sf::View* getView() const { return view.get(); }
	sf::Vector2f getWidgetOffset() const;
	sf::RenderStates getRenderStates() const { return states; }
	Player* getPlayer();
	std::weak_ptr<InputWidget> getActiveWidget() { return activeMenu; }
	void setIsPaused(const bool& bNewIsPaused)
	{
		bIsGameplayPaused = bNewIsPaused;
	}
	bool getIsPaused() const { return bIsGameplayPaused; }
	// Update Zoom (WIP)
	void setZoom(const float& newZoom) { view->zoom(zoomFactor = newZoom); }
	float getZoom() const { return zoomFactor; }
	// Input stuff
	bool handleEvent(sf::Event*);
	sf::Vector2f getMousePos() 
		{ return window->mapPixelToCoords(sf::Mouse::getPosition(*window)); }
	// GameState 
	void setGameState(const GameState&);
	GameState getGameState() const { return gameState; }

	W_Hud& getHud()
	{
		return validPlayer()->getHud();
	}

	void resetWindowName();
	void modWindowName(const std::string& suffix);

	void applySettings(const UserSettings_Struct settings);

	void setViewportValues(size_t resolutionID, bool bFullscreen); // For changing mid-game
	void setMaxFPS(unsigned int);
	void setUseVSync(bool);
	void setUseWidgetParallax(bool);
};

static inline GI_Arena& gameInstance()
{
	return GI_Arena::getInstance();
}