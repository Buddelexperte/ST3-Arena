#pragma once
#include "Button.h"
#include "BaseTypes.h"
#include "RenderInfo.h"

class GI_Arena;
class InputWidget;

// WIDGETS ----------------------------------------------------------------------------------------

class WidgetElement : public sf::Drawable, public IMovable
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

	virtual void tick(const float& deltaTime) { lastDeltaTime = deltaTime;  windowUpdate(); };
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

// SAVE GAME --------------------------------------------------------------------------------------


const std::string SAVE_FILE = "SaveGame.txt";

class SaveGame
{
public:
	static int Stored_Save;
	static int loadSavedData(const std::string&);
	static void saveData();
};