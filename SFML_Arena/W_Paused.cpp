#pragma once

#include "W_Paused.h" // Own header file
#include "GameInstance.h"

W_Paused::W_Paused(InputWidget* parent) 
	: InputWidget(parent), optionsMenu(this),
	pause_title(this), pause_resumeButton(this), pause_optionsButton(this), pause_quitButton(this)
{
	const std::vector<RawButton> PAUSED_CONSTR = {
		{sf::Vector2f{ 0.0f, -300.0f },    sf::Vector2f{ 350.0f, 120.0f }, sf::Color::Transparent,   100, "PAUSE",		sf::Color::White},
		{sf::Vector2f{ 0.0f, 0.0f },       sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "RESUME",		sf::Color::Black},
		{sf::Vector2f{ 0.0f, 150.0f },     sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "OPTIONS",	sf::Color::Black},
		{sf::Vector2f{ 0.0f, 300.0f },     sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "QUIT",		sf::Color::Black}
	};

	pause_title.construct(PAUSED_CONSTR[0]);
	pause_resumeButton.construct(PAUSED_CONSTR[1]);
	pause_optionsButton.construct(PAUSED_CONSTR[2]);
	pause_quitButton.construct(PAUSED_CONSTR[3]);
}

sf::Vector2f W_Paused::getCorrectTickCorrection() const
{
	return widgetOffset;
}

void W_Paused::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	pause_title.tick(deltaTime);
	pause_resumeButton.tick(deltaTime);
	pause_optionsButton.tick(deltaTime);
	pause_quitButton.tick(deltaTime);
}

void W_Paused::construct()
{
	InputWidget::construct();

	gameInstance().setGameState(GAME_PAUSED);

	setWidgetIndex(0);
}

InputWidget* W_Paused::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0:
		return this;
		break;
	case 1:
		return &optionsMenu;
		break;
	default:
		break;
	}
	return nullptr;
}

InputWidget* W_Paused::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0:
		shapes = { &pause_title, &pause_resumeButton, &pause_optionsButton, &pause_quitButton };
		break;
	default:
		shapes = { getActiveChild() };
		break;
	}
	return getActiveChild();
}

bool W_Paused::isMouseOver(const bool& checkForClick = false)
{
	std::cout << "isMouseOver" << std::endl;

	if (isChildActive())
		return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	if (pause_resumeButton.isMouseOver(checkForClick))
	{
		if (checkForClick) onKeyEscape();
		return true;
	}
	if (pause_optionsButton.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(1)->construct();
		return true;
	}
	if (pause_quitButton.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->playAnim(EAnimation::CLOSE_ANIM);
		return true;
	}
	// On no button-mouse overlap
	return false;
}