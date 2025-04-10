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
	InputWidget* parent;

public:
	WidgetElement(InputWidget* parentWidget);
	virtual ~WidgetElement() = default;

	virtual void construct() {}
	virtual void tick(const float& deltaTime) {}

	InputWidget* getParent() const { return parent; }
};

class InputWidget : public WidgetElement, public IHasInput
{
protected:
	int widgetIndex = 0;
	virtual bool onMouseClickL(sf::Event*) override { return isMouseOver(true); }

public:
	InputWidget(InputWidget* parent) 
		: WidgetElement(parent) {};
	~InputWidget() = default;

	// SubWidget ticking
	virtual void tick(const float& deltaTime) override
	{
		WidgetElement::tick(deltaTime);
		// Execute subWidgets
		if (isChildActive())
			getActiveChild()->tick(deltaTime);
	};

	// Input stuff
	virtual bool onKeyEscape() override;
	virtual bool onKeyTab() override;

	// Sub widget stuff
	virtual InputWidget* setWidgetIndex(const int&);
	virtual InputWidget* getWidgetAtIndex(const int& atIndex) { return (atIndex == 0 ? this : nullptr); };
	int getWidgetIndex() const { return widgetIndex; }
	bool isChildActive() const { return (widgetIndex > 0); }
	InputWidget* getActiveChild() { return getWidgetAtIndex(widgetIndex); }
};