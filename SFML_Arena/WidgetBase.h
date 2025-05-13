#pragma once
#include "BaseTypes.h"
#include "RenderInfo.h"
#include "Input.h"
#include "DrawableShape.h"
#include "Animation.h"

#include "Functions.h" // For sf::Vector2f maths

enum EAlignment
{
	LEFT_BOTTOM,
	LEFT,
	LEFT_TOP,
	CENTER_BOTTOM,
	CENTER,
	CENTER_TOP,
	RIGHT_BOTTOM,
	RIGHT,
	RIGHT_TOP
};

class InputWidget;

static inline constexpr bool NOT_INTERACTABLE_FLAG = true;

// WIDGETS ----------------------------------------------------------------------------------------

class WidgetElement : public IMovable, public IDrawableShapes, public IWidgetAnimation
{
private:
	sf::Vector2f tickPosCorrection = sf::Vector2f(0.0f, 0.0f); // Reference point for moving per tick to keep in View
protected:
	InputWidget* parent;
	// Reference point for moving per tick to keep in View
	virtual sf::Vector2f getTickCorrection() const;

public:
	WidgetElement(InputWidget* parentWidget);
	virtual ~WidgetElement() = default;

	virtual void construct();
	virtual void construct(const sf::Vector2f&) { construct(); }
	virtual void reset() {}
	virtual void tick(const float& deltaTime);

	virtual void tick_pos(const sf::Vector2f& withPos);

	InputWidget* getParent() const { return parent; }

	bool isAnimBlockingInput() const override;

	virtual bool isMouseOver(const bool& = false) { return false; }
};

class InputWidget : public WidgetElement, public IHasInput
{
private:
	// Used for relaying onClick logic and similar delegates of Buttons etc.
	virtual void delegateEvents() {};

protected:
	int widgetIndex = 0;
	virtual bool onMouseMoved(sf::Event*) override;
	virtual bool onMouseClickL(sf::Event*) override;

public:
	InputWidget(InputWidget* parent) 
		: WidgetElement(parent)
	{};
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
	virtual InputWidget* setWidgetIndex(const int& toIndex) { widgetIndex = toIndex; return getWidgetAtIndex(widgetIndex); };
	virtual InputWidget* getWidgetAtIndex(const int& atIndex) { return (atIndex == 0 ? this : nullptr); };
	int getWidgetIndex() const { return widgetIndex; }
	bool isChildActive() { return (getWidgetAtIndex(widgetIndex) != this); }
	InputWidget* getActiveChild() { return getWidgetAtIndex(widgetIndex); }
};