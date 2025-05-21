#include "LRButton.h"

void LRButton::constructLR(const RawButton& CONSTR, const bool startDisabled)
{
	RawButton leftConstr = {
		.pos = CONSTR.pos - sf::Vector2f(
			CONSTR.size.x / 6.0f,
			0.0f),
		.size = CONSTR.size * sf::Vector2f(1.0f/3.0f, 1.0f),
		.color = sf::Color::Transparent,
		.textSize = 24,
		.text = l_lable,
		.textColor = sf::Color::White,
		.alignment = EAlignment::RIGHT,
		.textAlignment = EAlignment::CENTER,
	};

	RawButton rightConstr = {
		.pos = CONSTR.pos + sf::Vector2f(
			CONSTR.size.x / 6.0f,
			0.0f),
		.size = CONSTR.size * sf::Vector2f(1.0f/3.0f, 1.0f),
		.color = sf::Color::Transparent,
		.textSize = 24,
		.text = r_lable,
		.textColor = sf::Color::White,
		.alignment = EAlignment::LEFT,
		.textAlignment = EAlignment::CENTER
	};

	left.construct(leftConstr, startDisabled);
	right.construct(rightConstr, startDisabled);
}

void LRButton::delegateEvents()
{
	left.onClick = [this]()
		{
			if (onLeftClick)
				onLeftClick();
		};

	right.onClick = [this]()
		{
			if (onRightClick)
				onRightClick();
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

void LRButton::tick(const float& deltaTime)
{
	left.tick(deltaTime);
	right.tick(deltaTime);
	Button::tick(deltaTime);
}

bool LRButton::isMouseOver(const bool& bCheckForClick)
{
	if (left.isMouseOver(bCheckForClick))
	{
		hover();
		return true;
	}

	if (right.isMouseOver(bCheckForClick))
	{
		hover();
		return true;
	}


	return Button::isMouseOver(bCheckForClick);
}
