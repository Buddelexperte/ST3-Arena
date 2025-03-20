#pragma once
#include "Button.h"
#include "BaseTypes.h"
#include "RenderInfo.h"
#include "Input.h"
#include "DrawableShape.h"

class GI_Arena;
class InputWidget;

// WIDGETS ----------------------------------------------------------------------------------------

class WidgetElement : public IMovable, public IDrawableShapes
{
protected:
	InputWidget* parent = nullptr;


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

	virtual void construct() {};

	InputWidget* getParent() const { return parent; }

	virtual void tick(const float& deltaTime) {};
};

class InputWidget : public WidgetElement, public IHasInput
{
protected:
	int widgetIndex = 0;
	virtual bool onMouseClickL(sf::Event*) override { return isMouseOver(true); }

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
	bool isChildActive() { return (widgetIndex > 0); }

	virtual bool handleEvent(sf::Event*) override;
	virtual void idleInputs() override;

	virtual sf::Keyboard::Key onKeyPressed(sf::Event*);
	virtual sf::Mouse::Button onMouseButtonPressed(sf::Event*);
	virtual sf::Mouse::Button onMouseButtonReleased(sf::Event*);

	virtual float onMouseScrolled(sf::Event*);

	virtual bool input_esc() override;
};