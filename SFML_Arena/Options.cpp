#pragma once

#include "Options.h" // Own header file

W_Options::W_Options(InputWidget* parent = nullptr) : InputWidget(parent), soundMenu(this), graphicMenu(this)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },		sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "OPTIONS",	sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 0 },			sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "SOUNDS",	sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 150 },		sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "GRAPHICS",	sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 300 },		sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "RETURN",	sf::Color::Black},
	};

	options_title.construct(MAIN_MENU_CONSTR[0]);
	options_graphics.construct(MAIN_MENU_CONSTR[1]);
	options_sounds.construct(MAIN_MENU_CONSTR[2]);
	options_return.construct(MAIN_MENU_CONSTR[3]);
}

void W_Options::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);
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
		shapes = { &options_title, &options_graphics, &options_return, &options_sounds };
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

	options_title.setPos(widgetOffset + sf::Vector2f{ 0, -300 });
	options_graphics.setPos(widgetOffset + sf::Vector2f{ 0, 0 });
	options_sounds.setPos(widgetOffset + sf::Vector2f{ 0, 150 });
	options_return.setPos(widgetOffset + sf::Vector2f{ 0, 300 });
}

bool W_Options::isMouseOver(const bool& checkForClick)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	if (options_return.isMouseOver(checkForClick))
	{
		if (checkForClick) onKeyEscape();
		return true;
	}
	if (options_graphics.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(1)->construct();
		return true;
	}
	if (options_sounds.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(2)->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}