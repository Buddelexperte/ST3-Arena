#pragma once

#include "W_OptionsGraphics.h" // Own header file

W_OptionsGraphics::W_OptionsGraphics(InputWidget* parent = nullptr) 
	: InputWidget(parent),
	optionsGraphics_return(this), optionsGraphics_test(this)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{sf::Vector2f{ 0, -300 },	sf::Vector2f{ 350, 120 }, sf::Color::Transparent,		100, "GRAPHICS", sf::Color::White},
		{sf::Vector2f{ 0, 300 },	sf::Vector2f{ 300, 100 }, sf::Color::White,         24,  "RETURN",   sf::Color::Black}

	};

	optionsGraphics_test.construct(MAIN_MENU_CONSTR[0]);
	optionsGraphics_return.construct(MAIN_MENU_CONSTR[1]);
}

void W_OptionsGraphics::construct()
{
	InputWidget::construct();
	shapes = { &optionsGraphics_test, &optionsGraphics_return };
}

void W_OptionsGraphics::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	optionsGraphics_test.tick(deltaTime);
	optionsGraphics_return.tick(deltaTime);
}

bool W_OptionsGraphics::isMouseOver(const bool& checkForClick = false)
{
	if (optionsGraphics_return.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}