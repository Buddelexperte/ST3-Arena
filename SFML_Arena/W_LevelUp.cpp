#pragma once

#include "W_LevelUp.h" // Own header file

W_LevelUp::W_LevelUp(InputWidget* parent)
	: InputWidget(parent)
{
	const std::vector<RawButton> LEVEL_UP_CONSTR = {
		// Background (bg)
		{viewCenter + sf::Vector2f{ 0.0f, 0.0f },	viewSize * 0.7f,	backgroundInterfaceColor,	0, "",	sf::Color::Transparent,	EAlignment::CENTER,	EAlignment::CENTER_TOP},
		{viewCenter + sf::Vector2f{ 0.0f, 0.0f },	viewSize * 0.7f,	backgroundInterfaceColor,	0, "",	sf::Color::Transparent,	EAlignment::CENTER,	EAlignment::CENTER_TOP},
		{viewCenter + sf::Vector2f{ 0.0f, 0.0f },	viewSize * 0.7f,	backgroundInterfaceColor,	0, "",	sf::Color::Transparent,	EAlignment::CENTER,	EAlignment::CENTER_TOP}
	};

	bg.construct(LEVEL_UP_CONSTR[0]);
	B_Cancel.construct(LEVEL_UP_CONSTR[1]);
	B_Submit.construct(LEVEL_UP_CONSTR[2]);
}

void W_LevelUp::construct()
{
	setWidgetIndex(0);
}

void W_LevelUp::tick(const float& deltaTime)
{
	bg.setPos(viewCenter + sf::Vector2f{ 0.0f, 0.0f });


}

InputWidget* W_LevelUp::setWidgetIndex(const int& toIndex)
{
	shapes = { &bg };

	switch ((widgetIndex = toIndex))
	{
	case 0: // SELF
		break;
	default:
		break;
	}

	return getWidgetAtIndex(widgetIndex);
}

InputWidget* W_LevelUp::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0: // SELF
		return this;
		break;
	default:
		break;
	}

	return nullptr;
}