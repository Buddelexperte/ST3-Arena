#pragma once

#include "W_OptionsGraphics.h" // Own header file

W_OptionsGraphics::W_OptionsGraphics(InputWidget* parent = nullptr) : InputWidget(parent)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 }, sf::Vector2f{ 350, 120 }, sf::Color::Transparent,		100, "GRAPHICS", sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 300 },		sf::Vector2f{ 300, 100 }, sf::Color::White,         24,  "RETURN",   sf::Color::Black}

	};

	optionsGraphics_test.construct(MAIN_MENU_CONSTR[0]);
	optionsGraphicsReturn.construct(MAIN_MENU_CONSTR[1]);
}

void W_OptionsGraphics::construct()
{
	InputWidget::construct();
	shapes = { &optionsGraphics_test, &optionsGraphicsReturn };
}

void W_OptionsGraphics::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
	optionsGraphics_test.setPosition(widgetOffset + sf::Vector2f{ 0, -300 });
}

bool W_OptionsGraphics::isMouseOver(const bool& checkForClick = false)
{
	if (optionsGraphicsReturn.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}