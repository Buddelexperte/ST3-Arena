#pragma once
#include "Button.h" // Button class for UI


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


// Global Game Instance ---------------------------------------------------------------------------


class GI_Arena // SINGLETON PATTERN
{
private:
	GI_Arena();
	GI_Arena(const GI_Arena&) = delete;
	GI_Arena& operator=(const GI_Arena&) = delete;

	sf::RenderWindow* window = nullptr;
	sf::RenderStates states;

	E_GameState gameState = QUIT;

	InputWidget* activeWidget = nullptr;
public:
	static GI_Arena& getInstance()
	{
		static GI_Arena instance;
		return instance;
	}
	void updateScreen();
	bool setActiveWidget(InputWidget* newActive)
	{
		bool bChanged = (newActive != activeWidget);
		activeWidget = newActive;
		return bChanged;
	}
	InputWidget* getActiveWidget() { return activeWidget; }
	sf::RenderWindow* getWindow() const { return window; }
	sf::RenderStates getRenderStates() const { return states; }
	sf::Vector2f getMousePos() { return static_cast<sf::Vector2f>(sf::Mouse::getPosition()); }
	E_GameState getGameState() const { return gameState; }
	void setGameState(const E_GameState&);
};


// WIDGETS ----------------------------------------------------------------------------------------


class WidgetElement : public sf::Drawable
{
protected:
	GI_Arena& gameInstance = GI_Arena::getInstance();
	sf::RenderWindow* window = gameInstance.getWindow();
	sf::Vector2u windowSize;
	sf::Vector2f windowCenter;
	std::vector<sf::Drawable*> shapes;
public:
	WidgetElement() { windowUpdate(); }

	virtual void setPosition(sf::Vector2f) {};
	virtual void setRotation(sf::Vector2f) {};
	virtual void setScale(sf::Vector2f) {};

	virtual void windowUpdate();
	virtual void construct() {};
	virtual void update(const float& deltaTime) { windowUpdate(); };
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class InputWidget : public WidgetElement
{
protected:
	sf::Event* event;
	virtual sf::Keyboard::Key keyboardInput(sf::Event* eventRef);
	virtual sf::Mouse::Button mouseInput(sf::Event* eventRef);
	virtual float scrollInput(sf::Event* eventRef);
	virtual bool onMouseClickL() { return isMouseOver(true); };
	virtual bool onMouseClickR() { return true; };
	virtual bool onMouseClickM() { return true; };
	virtual bool input_esc() { return true; };
public:
	bool handleInput(sf::Event* eventRef);
	virtual bool isMouseOver(const bool&) { return false; };
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