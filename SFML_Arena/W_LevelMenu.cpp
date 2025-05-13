#pragma once

#include "W_LevelMenu.h" // Own header file
#include "GameInstance.h"

W_LevelMenu::W_LevelMenu(InputWidget* parent) 
	: InputWidget(parent),
	T_Title(this), B_Level1(this), B_Level2(this), B_Level3(this), B_Return(this)
{
	const RawText TITLE_CONSTR = {
		sf::Vector2f(0.0f, -300.0f)* unitNorm, sf::Color::White, 100, "LEVEL SELECT"
	};

	const std::vector<RawButton> LEVEL_MENU_CONSTR = {
		{sf::Vector2f(-500.0f,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"LEVEL 1",		sf::Color::White},
		{sf::Vector2f(0.0f,		-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"LEVEL 2",		sf::Color::White},
		{sf::Vector2f(500.0f,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"LEVEL 3",		sf::Color::White},
		{sf::Vector2f(0.0f,		150.0f) * unitNorm,		buttonSize,									sf::Color::White,			24,		"RETURN",		sf::Color::Black}
	};

	T_Title.construct(TITLE_CONSTR);
	B_Level1.construct(LEVEL_MENU_CONSTR[0]);
	B_Level2.construct(LEVEL_MENU_CONSTR[1]);
	B_Level3.construct(LEVEL_MENU_CONSTR[2]);
	B_Return.construct(LEVEL_MENU_CONSTR[3]);

	delegateEvents();

	shapes = { &T_Title, &B_Level1, &B_Level2, &B_Level3, &B_Return };
}

void W_LevelMenu::delegateEvents()
{
	B_Level1.onClick = [this]()
	{
		selectLevel(1);
	};

	B_Level2.onClick = [this]()
	{
		selectLevel(2);
	};

	B_Level3.onClick = [this]()
	{
		selectLevel(3);
	};

	B_Return.onClick = [this]()
	{
		onKeyEscape();
	};
}


void W_LevelMenu::construct()
{
	InputWidget::construct();

	gameInstance().modWindowName(" - Selecting Level...");
	setWidgetIndex(0);

	T_Title.construct();
	B_Level1.construct();
	B_Level2.construct();
	B_Level3.construct();
	B_Return.construct();
}

void W_LevelMenu::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);
	B_Level1.tick(deltaTime);
	B_Level2.tick(deltaTime);
	B_Level3.tick(deltaTime);
	B_Return.tick(deltaTime);
}


bool W_LevelMenu::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	if (B_Level1.isMouseOver(checkForClick) 
		|| B_Level2.isMouseOver(checkForClick) 
		|| B_Level3.isMouseOver(checkForClick))
		return true;

	if (B_Return.isMouseOver(checkForClick))
		return true;

	return false;
}

void W_LevelMenu::selectLevel(const unsigned int level)
{
	// TODO: Add different level select to have consequences
	parent->setWidgetIndex(parent->getWidgetIndex() + 1)->construct();
}