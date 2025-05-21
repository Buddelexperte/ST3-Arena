#include "LRButton.h"
//cool
void LRButton::constructLR(const RawButton& CONSTR, const bool startDisabled)
{
	RawButton leftConstr = {
		.pos = CONSTR.pos - sf::Vector2f(
			CONSTR.size.x / 2.0f,
			0.0f),
		.size = CONSTR.size * sf::Vector2f(1.0f/4.0f, 1.0f),
		.color = sf::Color::Transparent,
		.textSize = 24,
		.text = l_lable,
		.textColor = CONSTR.textColor,
		.alignment = EAlignment::LEFT,
		.textAlignment = EAlignment::CENTER,
	};

	RawButton rightConstr = {
		.pos = CONSTR.pos + sf::Vector2f(
			CONSTR.size.x / 2.0f,
			0.0f),
		.size = CONSTR.size * sf::Vector2f(1.0f/4.0f, 1.0f),
		.color = sf::Color::Transparent,
		.textSize = 24,
		.text = r_lable,
		.textColor = CONSTR.textColor,
		.alignment = EAlignment::RIGHT,
		.textAlignment = EAlignment::CENTER
	};

	left.construct(leftConstr, startDisabled);
	right.construct(rightConstr, startDisabled);
}

void LRButton::delegateEvents()
{
	// Left

	left.onClick = [this]()
		{
			if (onLeftClick)
				onLeftClick();
		};

	left.onUnhover = [this]()
		{
			const unsigned int currTextSize = getButtonData().textSize;
			left.setTextSize(currTextSize);
		};

	left.onHover = [this]()
		{
			std::cout << "l hovered" << std::endl;
			const unsigned int currTextSize = getButtonData().textSize;
			left.setTextSize(currTextSize + 20);
		};

	// Right

	right.onClick = [this]()
		{
			if (onRightClick)
				onRightClick();
		};

	right.onUnhover = [this]()
		{
			const unsigned int currTextSize = getButtonData().textSize;
			right.setTextSize(currTextSize);
		};

	right.onHover = [this]()
		{
			std::cout << "r hovered" << std::endl;
			const unsigned int currTextSize = getButtonData().textSize;
			right.setTextSize(currTextSize + 20);
		};
}

void LRButton::construct()
{
	construct(getButtonData());
}

void LRButton::construct(const RawButton& CONSTR)
{
	construct(CONSTR, getEnabled());
}

void LRButton::construct(const RawButton& CONSTR, const bool startDisabled)
{
	// RLButton doesn't support alignment yet, defaults to CENTER
	RawButton actualConstr = CONSTR;
	actualConstr.alignment = EAlignment::CENTER;

	constructLR(actualConstr, startDisabled);

	Button::construct(actualConstr, startDisabled);
}

void LRButton::tick(const float& deltaTime)
{
	left.tick(deltaTime);
	right.tick(deltaTime);
	Button::tick(deltaTime);
}

bool LRButton::isMouseOver(const bool& bCheckForClick)
{
	bool middleOver = Button::isMouseOver(bCheckForClick);

	if (left.isMouseOver(bCheckForClick))
		return true;

	if (right.isMouseOver(bCheckForClick))
		return true;


	return middleOver;
}
