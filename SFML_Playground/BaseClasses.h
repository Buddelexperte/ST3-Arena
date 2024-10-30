#pragma once

const std::string SAVE_FILE = "../SaveGame.txt";

class SaveGame
{
private:
public:
	static int Stored_Save;
	static int loadSavedData(const std::string& path = SAVE_FILE);
	static void saveData();
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
	sf::RenderStates getRenderStates() const { return states; }
	E_GameState getGameState() const { return gameState; }
	void setGameState(const E_GameState&);
	bool getGameStateChanges(E_GameState& oldGS) const;
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
	WidgetMenu() { windowUpdate(); }
	virtual void windowUpdate();
	virtual void init() {};
	virtual void update(const float& deltaTime) { windowUpdate(); };
	virtual bool isInteracted(const sf::Vector2f& mousePos) { return false; };
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};