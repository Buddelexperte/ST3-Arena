#pragma once
#include "Button.h"
#include "BaseTypes.h"

class GI_Arena;
class InputWidget;

// WIDGETS ----------------------------------------------------------------------------------------


class WidgetElement : public sf::Drawable
{
protected:
	float lastDeltaTime = 0.0f;
	InputWidget* parent = nullptr;
	GI_Arena* gameInstance;
	sf::RenderWindow* window;
	sf::Vector2u windowSize;
	sf::Vector2f windowCenter;
	sf::Vector2f ORIGIN;
	sf::View* view = nullptr;
	sf::Vector2f viewSize;
	sf::Vector2f viewHalfSize;
	sf::Vector2f viewCenter;
	std::vector<sf::Drawable*> shapes;

	virtual void windowUpdate();
public:
	WidgetElement(InputWidget* parentWidget);
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
	virtual InputWidget* getWidgetAtIndex(const int& atIndex) { return (atIndex == 0 ? this : nullptr); };
	int getWidgetIndex() const { return widgetIndex; }

	virtual bool input_esc()
	{
		if (getWidgetAtIndex(widgetIndex) != this) return getWidgetAtIndex(widgetIndex)->input_esc();
		if (parent != nullptr) parent->setWidgetIndex(0)->construct();
		else return false;
		return true;
	}
	virtual bool handleInput(sf::Event* eventRef);
	virtual bool isMouseOver(const bool& = false) { return false; }
};


// PLAYER -----------------------------------------------------------------------------------------

class Player : public InputWidget
{
private:
	Button playerModel;
	sf::Vector2f velocity = { 0.0f, 0.0f };
	sf::Vector2f direction = { 0.0f, 0.0f };

	std::vector<sf::Texture> playerTexture = {};
	int currentframe;
	float animationAccu;
	float animationSpeed;

	void calcMovement(const float&);
protected:
	sf::Keyboard::Key keyboardInput(sf::Event*) override;
	sf::Mouse::Button mouseInput(sf::Event*) override;
	float scrollInput(sf::Event*) override;
public:
	Player(InputWidget*);
	void update(const float&) override;
	void setPos(const sf::Vector2f&) override;
	void addPos(const sf::Vector2f&) override;
	sf::Vector2f getPos() const override;
	void setRot(const float&) override;
	float getRot() const override;
	sf::Vector2f getVelocity() const { return velocity; };
	sf::Vector2f getDirection() const { return direction; };
};


// SAVE GAME --------------------------------------------------------------------------------------


const std::string SAVE_FILE = "SaveGame.txt";

class SaveGame
{
public:
	static int Stored_Save;
	static int loadSavedData(const std::string&);
	static void saveData();
};