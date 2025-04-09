#pragma once

#include <iostream> // For debug messages

#include "MainMenu.h" // Own header file
#include "GameInstance.h"
#include "SaveGame.h"

W_MainMenu::W_MainMenu(InputWidget* parent)
	: InputWidget(parent), optionsMenu(this), levelMenu(this), selectWeapon(this)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100,	"ARENA",								sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, -200 },    sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16,	"Higscore: " + std::to_string(0),		sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24,	"START",								sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24,	"OPTIONS",								sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24,	"QUIT",									sf::Color::Black}
	};

	menu_title.construct(MAIN_MENU_CONSTR[0]);
	menu_highscore.construct(MAIN_MENU_CONSTR[1]);
	menu_startButton.construct(MAIN_MENU_CONSTR[2]);
	menu_optionsButton.construct(MAIN_MENU_CONSTR[3]);
	menu_quitButton.construct(MAIN_MENU_CONSTR[4]);

	// Done out
	std::cout << "- Constructed MainMenu" << std::endl;
}

void W_MainMenu::construct()
{
	setWidgetIndex(0);
	menu_highscore.setText("Highscore: " + std::to_string(SaveGame::storedData.score));
}

InputWidget* W_MainMenu::getWidgetAtIndex(const int& index)
{
	switch (index)
	{
	case 0:
		return this; // Self
		break;
	case 1:
		return &optionsMenu; // OPTIONS_MENU
		break;
	case 2:
		return &levelMenu; // LEVEL_MENU
		break;
	case 3:
		return &selectWeapon; // SELECT WEAPON SCREEN
	default:
		break;
	}
	return nullptr;
}

InputWidget* W_MainMenu::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0: // MAIN_MENU
		shapes = { &menu_title, &menu_highscore, &menu_startButton, &menu_optionsButton, &menu_quitButton };
		break;
	default: // SUB-WIDGETS (implement this notation in EVERY other Widget)
		shapes = { getActiveChild() };
		break;
	}
	return getActiveChild();
}

void W_MainMenu::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	menu_title.setPos(widgetOffset + sf::Vector2f{ 0, -300 });
	menu_highscore.setPos(widgetOffset + sf::Vector2f{ 0, -200 });
	menu_startButton.setPos(widgetOffset + sf::Vector2f{ 0, 0 });
	menu_optionsButton.setPos(widgetOffset + sf::Vector2f{ 0, 150 });
	menu_quitButton.setPos(widgetOffset + sf::Vector2f{ 0, 300 });
}

bool W_MainMenu::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	sf::Vector2f mousePos = gameInstance().getMousePos();

	if (menu_title.isMouseOver(checkForClick))
	{
		sf::Color newColor = (menu_title.getColor(true) == sf::Color::White ? sf::Color::Red : sf::Color::White);
		if (checkForClick) menu_title.setColor(newColor, true);
		return true;
	}
	if (menu_startButton.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(2)->construct();
		return true;
	}
	if (menu_optionsButton.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(1)->construct();
		return true;
	}
	if (menu_quitButton.isMouseOver(checkForClick))
	{
		if (checkForClick) gameInstance().setGameState(QUIT);
		return true;
	}
	// On no button-mouse overlap
	return false;
}

bool W_MainMenu::input_esc()
{
	if (isChildActive())
		getActiveChild()->input_esc();
	else
		gameInstance().setGameState(QUIT);
	return true;
}