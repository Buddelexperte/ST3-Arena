#pragma once
#include <iostream>
#include "Functions.h"
#include "Button.h"

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
class WidgetElement;
class InputWidget;
class Player;

// Global Game Instance ---------------------------------------------------------------------------


class GI_Arena // SINGLETON PATTERN
{
private:
	GI_Arena();
	GI_Arena(const GI_Arena&) = delete;
	GI_Arena& operator=(const GI_Arena&) = delete; // Block the '=' operator to stop copies being made of this class

	sf::RenderWindow* window = nullptr;
	sf::View* view = nullptr;
	sf::RenderStates states;

	E_GameState gameState = MENU_SCREEN;

	InputWidget* activeWidget = nullptr;
	Player* playerRef = nullptr;

	bool bIsGameplayPaused = true;
public:
	static GI_Arena& getInstance()
	{
		static GI_Arena instance;
		return instance;
	}
	void setIsPaused(const bool& bPause) { bIsGameplayPaused = bPause; }
	bool getIsPaused() const { return bIsGameplayPaused; }
	void updateScreen();
	void setViewCenter(const sf::Vector2f&);
	void tick(const float&);
	bool setActiveWidget(InputWidget*);
	sf::RenderWindow* getWindow() const { return window; }
	sf::View* getView() const { return view; }
	InputWidget* getActiveWidget() { return activeWidget; }
	Player* getPlayer();
	bool handleEvent(sf::Event*);
	sf::RenderStates getRenderStates() const { return states; }
	sf::Vector2f getMousePos() { return window->mapPixelToCoords(sf::Mouse::getPosition(*window)); }
	E_GameState getGameState() const { return gameState; }
	void setGameState(const E_GameState&);
};


// WIDGETS ----------------------------------------------------------------------------------------


class WidgetElement : public sf::Drawable
{
protected:
	float lastDeltaTime = 0.0f;
	InputWidget* parent = nullptr;
	GI_Arena& gameInstance = GI_Arena::getInstance();
	sf::RenderWindow* window = gameInstance.getWindow();
	sf::Vector2u windowSize;
	sf::Vector2f windowCenter;
	sf::Vector2f ORIGIN = window->getView().getCenter();
	sf::View* view = nullptr;
	sf::Vector2f viewCenter;
	std::vector<sf::Drawable*> shapes;

	virtual void windowUpdate();
public:
	WidgetElement(InputWidget* parentWidget) : parent(parentWidget)
	{
		windowUpdate();
	}
	virtual ~WidgetElement()
	{
		for (sf::Drawable* drawable : shapes)
		{
			drawable = nullptr;
		}
		window = nullptr;
		view = nullptr;
		parent = nullptr;
	}
	virtual void construct() { windowUpdate(); };

	InputWidget* getParent() const { return parent; }
	// Position
	virtual void setPos(const sf::Vector2f&) { return; }
	virtual void addPos(const sf::Vector2f&) { return; }
	virtual sf::Vector2f getPos() const { return sf::Vector2f(0.0f, 0.0f); };
	// Rotation
	virtual void setRot(const float&) { return; }
	virtual void addRot(const float&) { return; }
	virtual float getRot() const { return 0.0f; }
	// Scale (NOT SIZE)
	virtual void setScale(const sf::Vector2f&) { return; };
	virtual sf::Vector2f getScale() const { return sf::Vector2f(0.0f, 0.0f); };

	virtual void update(const float& deltaTime) { lastDeltaTime = deltaTime;  windowUpdate(); };
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class InputWidget : public WidgetElement
{
protected:
	sf::Event* event = nullptr;

	int widgetIndex = 0;

	virtual sf::Keyboard::Key keyboardInput(sf::Event*);
	virtual sf::Mouse::Button mouseInput(sf::Event*);
	virtual float scrollInput(sf::Event*);
	virtual bool onMouseClickL() { return isMouseOver(true); };
	virtual bool onMouseClickR() { return true; };
	virtual bool onMouseClickM() { return true; };
public:
	InputWidget(InputWidget* parent) : WidgetElement(parent) {};

	virtual InputWidget* setWidgetIndex(const int&);
	virtual InputWidget* getWidgetAtIndex(const int&);
	int getWidgetIndex() const { return widgetIndex; }

	virtual bool input_esc() { parent->setWidgetIndex(0)->construct(); return true; }
	virtual bool handleInput(sf::Event* eventRef);
	virtual bool isMouseOver(const bool&) { return false; }
};


// PLAYER -----------------------------------------------------------------------------------------

class Player : public InputWidget
{
private:
	Button playerModel;
	sf::Vector2f velocity = { 0.0f, 0.0f };

	void calcMovement(const float&);
protected:
	sf::Keyboard::Key keyboardInput(sf::Event*) override;
	sf::Mouse::Button mouseInput(sf::Event*) override;
public:
	Player(InputWidget*);
	void update(const float&) override;
	void setPos(const sf::Vector2f&) override;
	void addPos(const sf::Vector2f&) override;
	sf::Vector2f getPos() const override;
	void setRot(const float&) override;
	float getRot() const override;
};


// SAVE GAME --------------------------------------------------------------------------------------


const std::string SAVE_FILE = "../SaveGame.txt";

class SaveGame
{
public:
	static int Stored_Save;
	static int loadSavedData(const std::string&);
	static void saveData();
};