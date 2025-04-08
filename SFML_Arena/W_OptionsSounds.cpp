#pragma once

#include "W_OptionsSounds.h" // Own header file
#include "GameInstance.h"

W_OptionsSounds::W_OptionsSounds(InputWidget* parent = nullptr) : InputWidget(parent)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "SOUND",		sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 300 },		sf::Vector2f{ 300, 100 }, sf::Color::White,        24, "RETURN",	sf::Color::Black}
	};

	optionsSounds_test.construct(MAIN_MENU_CONSTR[0]);
	optionsSoundsReturn.construct(MAIN_MENU_CONSTR[1]);
}

void W_OptionsSounds::construct()
{
	InputWidget::construct();
	shapes = { &optionsSounds_test, &optionsSoundsReturn };
}

void W_OptionsSounds::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
	optionsSounds_test.setPos(widgetOffset + sf::Vector2f{ 0, -300 });
}

bool W_OptionsSounds::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = gameInstance().getMousePos();
	if (optionsSoundsReturn.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}