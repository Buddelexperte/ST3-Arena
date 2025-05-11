#pragma once

#include "W_Paused.h" // Own header file
#include "GameInstance.h"

W_Paused::W_Paused(InputWidget* parent) 
	: InputWidget(parent), optionsMenu(this),
	T_Title(this), B_Resume(this), B_OpenOptions(this), B_Quit(this)
{
	const RawText TITLE_CONSTR = {
		sf::Vector2f{ 0.0f, -300.0f }, sf::Color::White, 100, "PAUSE"
	};

	const std::vector<RawButton> PAUSED_CONSTR = {
		{sf::Vector2f{ 0.0f, 0.0f },       sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "RESUME",		sf::Color::Black},
		{sf::Vector2f{ 0.0f, 150.0f },     sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "OPTIONS",	sf::Color::Black},
		{sf::Vector2f{ 0.0f, 300.0f },     sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "QUIT",		sf::Color::Black}
	};

	T_Title.construct(TITLE_CONSTR);
	B_Resume.construct(PAUSED_CONSTR[0]);
	B_OpenOptions.construct(PAUSED_CONSTR[1]);
	B_Quit.construct(PAUSED_CONSTR[2]);

	delegateButtons();
}

void W_Paused::delegateButtons()
{
	B_Resume.onClick = [this]()
	{
		onKeyEscape();
	};

	B_OpenOptions.onClick = [this]()
	{
		setWidgetIndex(1)->construct();
	};

	B_Quit.onClick = [this]()
	{
		parent->playAnim(EAnimation::CLOSE_ANIM);
	};
}

void W_Paused::construct()
{
	InputWidget::construct();

	gameInstance().modWindowName(" (Pausiert)");
	gameInstance().setGameState(GAME_PAUSED);

	setWidgetIndex(0);
}

void W_Paused::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);
	B_Resume.tick(deltaTime);
	B_OpenOptions.tick(deltaTime);
	B_Quit.tick(deltaTime);
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
		shapes = { &T_Title, &B_Resume, &B_OpenOptions, &B_Quit };
		break;
	default:
		shapes = { getActiveChild() };
		break;
	}
	return getActiveChild();
}

bool W_Paused::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	if (B_Resume.isMouseOver(checkForClick))
		return true;

	if (B_OpenOptions.isMouseOver(checkForClick))
		return true;

	if (B_Quit.isMouseOver(checkForClick))
		return true;

	// On no button-mouse overlap
	return false;
}