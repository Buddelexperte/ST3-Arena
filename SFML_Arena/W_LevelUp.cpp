#pragma once

#include "W_LevelUp.h" // Own header file
#include "functions.h"

W_LevelUp::W_LevelUp(InputWidget* parent)
	: InputWidget(parent)
{
	const std::vector<RawButton> LEVEL_UP_CONSTR = {
		// Background (bg)
		{viewCenter + sf::Vector2f{ 0.0f, 0.0f },					sf::Vector2f{ 2000.0f, 1000.0f } * viewSizeNorm,	backgroundInterfaceColor,		0,	"",			sf::Color::Transparent,			EAlignment::CENTER,	EAlignment::CENTER_TOP},
		{viewCenter + (sf::Vector2f{ 0, 480.0f } * viewSizeNorm),	buttonSize,											sf::Color(255, 255, 255, 255),	24,	" S K I P",	sf::Color::Black,	EAlignment::CENTER_BOTTOM,	EAlignment::CENTER},
	};

	bg.construct(LEVEL_UP_CONSTR[0]);
	B_Cancel.construct(LEVEL_UP_CONSTR[1]);
}

void W_LevelUp::construct()
{
	setWidgetIndex(0);
}

void W_LevelUp::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	bg.setPos(widgetOffset + sf::Vector2f{ 0.0f, 0.0f });
	B_Cancel.setPos(widgetOffset + (sf::Vector2f{ 0, 480.0f } * viewSizeNorm));
}

InputWidget* W_LevelUp::setWidgetIndex(const int& toIndex)
{
	shapes = { &bg, &B_Cancel };

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