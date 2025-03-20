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

	virtual void tick(const float& deltaTime) 
	{ 
		lastDeltaTime = deltaTime;  
		windowUpdate(); 
	};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class InputWidget : public WidgetElement
{
protected:
	sf::Event* event = nullptr;

	int widgetIndex = 0;

	virtual sf::Keyboard::Key onKeyPressed(sf::Event*);
	virtual sf::Mouse::Button onMouseButtonPressed(sf::Event*);
	virtual sf::Mouse::Button onMouseButtonReleased(sf::Event*);
	virtual float onMouseScrolled(sf::Event*);
	virtual bool onMouseClickL(sf::Event*) { return isMouseOver(true); };
	virtual bool onMouseClickR(sf::Event*) { return true; };
	virtual bool onMouseClickM(sf::Event*) { return true; };
	virtual void onMouseDownL() {};
	virtual void onMouseDownR() {};
	virtual void onMouseDownM() {};
	virtual bool onMouseReleaseL(sf::Event*) { return true; };
	virtual bool onMouseReleaseR(sf::Event*) { return true; };
	virtual bool onMouseReleaseM(sf::Event*) { return true; };
public:
	InputWidget(InputWidget* parent) : WidgetElement(parent) {};

	virtual void tick(const float& deltaTime) override
	{
		WidgetElement::tick(deltaTime);
		// Execute subWidgets
		if (isChildActive())
			getWidgetAtIndex(widgetIndex)->tick(deltaTime);
	};

	virtual InputWidget* setWidgetIndex(const int&);
	virtual InputWidget* getWidgetAtIndex(const int& atIndex) { return (atIndex == 0 ? this : nullptr); };
	int getWidgetIndex() const { return widgetIndex; }
	bool isChildActive() { return (getWidgetAtIndex(widgetIndex) != this); }

	virtual bool input_esc()
	{
		if (getWidgetAtIndex(widgetIndex) != this) return getWidgetAtIndex(widgetIndex)->input_esc();
		if (parent != nullptr) parent->setWidgetIndex(0)->construct();
		else return false;
		return true;
	}
	virtual bool handleEvent(sf::Event* eventRef);
	virtual void idleInputs();
	virtual bool isMouseOver(const bool& = false) { return false; }
};