#pragma once

#include "Options.h" // Own header file
#include "SoundManager.h"
#include "GameInstance.h"

W_Options::W_Options(InputWidget* parent) 
	: InputWidget(parent), 
	soundMenu(this), graphicMenu(this),
	T_Title(this), B_Graphics(this), B_Sounds(this), B_Return(this), T_DevMode(this)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{sf::Vector2f{ 0, -300 },										buttonSize,	sf::Color::Transparent,		100,	"OPTIONS",		sf::Color::White},
		{sf::Vector2f{ 0, 0 },											buttonSize,	sf::Color::White,			24,		"SOUNDS",		sf::Color::Black},
		{sf::Vector2f{ 0, 150 },										buttonSize,	sf::Color::White,			24,		"GRAPHICS",		sf::Color::Black},
		{sf::Vector2f{ 0, 300 },										buttonSize,	sf::Color::White,			24,		"RETURN",		sf::Color::Black},
		{sf::Vector2f(viewSize.x, -viewSize.y) / 2.0f,		buttonSize,	sf::Color::Transparent,		24,		"",				sf::Color::Red,		EAlignment::RIGHT_TOP, EAlignment::RIGHT_TOP}
	};

	T_Title.construct(MAIN_MENU_CONSTR[0], NOT_INTERACTABLE_FLAG);
	B_Graphics.construct(MAIN_MENU_CONSTR[1]);
	B_Sounds.construct(MAIN_MENU_CONSTR[2]);
	B_Return.construct(MAIN_MENU_CONSTR[3]);
	T_DevMode.construct(MAIN_MENU_CONSTR[4], NOT_INTERACTABLE_FLAG);

	delegateButtons();
}

void W_Options::delegateButtons()
{
	B_Return.onClick = [this]() {
		onKeyEscape();
		};

	B_Graphics.onClick = [this]() {
		setWidgetIndex(1)->construct();
		};

	B_Sounds.onClick = [this]() {
		setWidgetIndex(2)->construct();
		};
}

void W_Options::construct()
{
	InputWidget::construct();

	setWidgetIndex(0);

	if (gameInstance().getIsDebugMode()) T_DevMode.setText("DEBUG MODE");

	T_Title.construct();
	B_Graphics.construct();
	B_Sounds.construct();
	B_Return.construct();
	T_DevMode.construct();
}

InputWidget* W_Options::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0: // OPTIONS SELECTOR
		return this;
		break;
	case 1: // SOUNDS
		return &soundMenu;
		break;
	case 2: // GRAPHICS
		return &graphicMenu;
		break;
	default:
		break;
	}

	return nullptr;
}

InputWidget* W_Options::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0: // THIS
		shapes = { &T_Title, &B_Graphics, &B_Return, &B_Sounds };
		break;
	case 1: // SOUNDS
		shapes = { &soundMenu };
		break;
	case 2: // GRAPHICS
		shapes = { &graphicMenu };
		break;
	default:
		shapes = { };
		break;
	}

	shapes.push_back(&T_DevMode);

	return getActiveChild();
}

void W_Options::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);
	B_Graphics.tick(deltaTime);
	B_Sounds.tick(deltaTime);
	B_Return.tick(deltaTime);
	T_DevMode.tick(deltaTime);
}

bool W_Options::isMouseOver(const bool& checkForClick)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	if (B_Return.isMouseOver(checkForClick))
		return true;

	if (B_Graphics.isMouseOver(checkForClick))
		return true;

	if (B_Sounds.isMouseOver(checkForClick))
		return true;

	// On no button-mouse overlap
	return false;
}