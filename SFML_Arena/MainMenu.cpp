#pragma once

#include <iostream> // For debug messages

#include "MainMenu.h" // Own header file
#include "GameInstance.h"

W_MainMenu::W_MainMenu(InputWidget* parent)
	: InputWidget(parent),
	fadeScreen(this),
	titleMenu(this), optionsMenu(this), levelMenu(this), selectWeapon(this)
{
	fadeScreen.setPosition(viewTL);
	fadeScreen.setSize(viewSize);
	
	// Done out
	std::cout << "- Constructed MainMenu" << std::endl;
}

void W_MainMenu::construct()
{
	std::cout << "Constructed MainMenu" << std::endl;

	InputWidget::construct();

	gameInstance().setIsPaused(false);

	setWidgetIndex(0)->construct();
}

InputWidget* W_MainMenu::getWidgetAtIndex(const int& index)
{
	switch (index)
	{
	case 0:
		return &titleMenu; // TITLE_MENU
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
	widgetIndex = newIndex;

	shapes = { getActiveChild(), &fadeScreen };

	return getActiveChild();
}

void W_MainMenu::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	fadeScreen.tick(deltaTime);
}

bool W_MainMenu::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	// On no button-mouse overlap
	return false;
}

sf::Vector2f W_MainMenu::getCorrectTickCorrection() const
{
	return widgetOffset;
}

bool W_MainMenu::onKeyEscape()
{
	if (isChildActive())
		getActiveChild()->onKeyEscape();
	else
		gameInstance().setGameState(QUIT);
	return true;
}

void W_MainMenu::start_openAnim()
{
	fadeScreen.setFadeColor(sf::Color::Black, sf::Color::Transparent, SCREEN_FADE_DURATION);
	fadeScreen.startFade();
}

void W_MainMenu::start_closeAnim()
{
	fadeScreen.setFadeColor(sf::Color::Transparent, sf::Color::Black, SCREEN_FADE_DURATION);
	fadeScreen.startFade();
}

void W_MainMenu::tick_openAnim(const float&)
{
	if (!fadeScreen.isFading())
	{
		IWidgetAnimation::stopAnim();
	}
}

void W_MainMenu::tick_closeAnim(const float& deltaTime)
{
	if (!fadeScreen.isFading())
	{
		gameInstance().launchGame();
		IWidgetAnimation::stopAnim();
	}
}
