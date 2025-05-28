#pragma once

#include "Options.h" // Own header file
#include "SoundManager.h"
#include "GameInstance.h"

W_Options::W_Options(InputWidget* parent) 
	: InputWidget(parent), 
	soundMenu(this), graphicMenu(this),
	T_Title(this), B_Graphics(this), B_Sounds(this), B_Return(this)
{
	const RawText TITLE_CONSTR = {
		sf::Vector2f{ 0, -300 }, sf::Color::White, 100, "OPTIONS", EAlignment::CENTER
	};

	const std::vector<RawButton> BUTTON_CONSTR = {
		{sf::Vector2f{ 0, 0 },		buttonSize,	darkerButtonColor,			24,		"SOUNDS",		normalTextColor},
		{sf::Vector2f{ 0, 150 },	buttonSize,	darkerButtonColor,			24,		"GRAPHICS",		normalTextColor},
		{sf::Vector2f{ 0, 300 },	buttonSize,	darkerButtonColor,			24,		"RETURN",		normalTextColor}
	};

	T_Title.construct(TITLE_CONSTR);
	B_Graphics.construct(BUTTON_CONSTR[0]);
	B_Graphics.setTexture(buttonTexture);
	B_Sounds.construct(BUTTON_CONSTR[1]);
	B_Sounds.setTexture(buttonTexture);
	B_Return.construct(BUTTON_CONSTR[2]);
	B_Return.setTexture(buttonTexture);

	delegateEvents();
}

void W_Options::delegateEvents()
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

	T_Title.construct();
	B_Graphics.construct();
	B_Sounds.construct();
	B_Return.construct();
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

	return getActiveChild();
}

void W_Options::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);
	B_Graphics.tick(deltaTime);
	B_Sounds.tick(deltaTime);
	B_Return.tick(deltaTime);
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