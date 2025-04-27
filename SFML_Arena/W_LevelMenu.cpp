#pragma once

#include "W_LevelMenu.h" // Own header file

W_LevelMenu::W_LevelMenu(InputWidget* parent) : InputWidget(parent)
{
	const std::vector<RawButton> LEVEL_MENU_CONSTR = {
		{viewCenter + sf::Vector2f(0.0f, -300.0f),	sf::Vector2f(100.0f, 100.0f),	sf::Color::Transparent,		100,	"LEVEL SELECT", sf::Color::White},
		{viewCenter + sf::Vector2f(-500.0f, 0.0f),	sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"LEVEL 1",		sf::Color::White},
		{viewCenter + sf::Vector2f(0.0f, 0.0f),		sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"LEVEL 2",		sf::Color::White},
		{viewCenter + sf::Vector2f(500.0f, 0.0f),	sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"LEVEL 3",		sf::Color::White},
		{viewCenter + sf::Vector2f(0.0f, 300.0f),	sf::Vector2f(200.0f, 100.0f),	sf::Color::White,			24,		"RETURN",		sf::Color::Black}
	};

	levelmenu_title.construct(LEVEL_MENU_CONSTR[0]);
	level1_Button.construct(LEVEL_MENU_CONSTR[1]);
	level2_Button.construct(LEVEL_MENU_CONSTR[2]);
	level3_Button.construct(LEVEL_MENU_CONSTR[3]);
	return_Button.construct(LEVEL_MENU_CONSTR[4]);
}

void W_LevelMenu::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	levelmenu_title.setPosition(widgetOffset + sf::Vector2f(0.0f, -300.0f));
	level1_Button.setPosition(widgetOffset + sf::Vector2f(-500.0f, 0.0f));
	level2_Button.setPosition(widgetOffset + sf::Vector2f(0.0f, 0.0f));
	level3_Button.setPosition(widgetOffset + sf::Vector2f(500.0f, 0.0f));
	return_Button.setPosition(widgetOffset + sf::Vector2f(0.0f, 300.0f));
}

void W_LevelMenu::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);
}

bool W_LevelMenu::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	if (level1_Button.isMouseOver(checkForClick) || level2_Button.isMouseOver(checkForClick) || level3_Button.isMouseOver(checkForClick))
	{
		if (checkForClick)
		{
			parent->setWidgetIndex(parent->getWidgetIndex() + 1)->construct();
			return true;
		}
	}
	if (return_Button.isMouseOver(checkForClick))
	{
		if (checkForClick) onKeyEscape();
		return true;
	}
	return false;
}

InputWidget* W_LevelMenu::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0:
		shapes = { &levelmenu_title, &level1_Button, &level2_Button, &level3_Button, &return_Button };
		break;
	default:
		shapes = { getWidgetAtIndex(widgetIndex) };
		break;
	}
	return getWidgetAtIndex(widgetIndex);
}