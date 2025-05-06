#pragma once

#include "W_OptionsGraphics.h" // Own header file

W_OptionsGraphics::W_OptionsGraphics(InputWidget* parent = nullptr) 
	: InputWidget(parent),
	optionsGraphics_return(this), optionsGraphics_test(this), optionsGraphics_Resolution(this), optionsGraphics_FPSLimit(this),
	optionsGraphics_Vsync(this), optionsGraphics_Apply(this), optionsGraphics_MenuParallax(this)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{sf::Vector2f{ 0, -300 },		buttonSize,		sf::Color::Transparent,			100, "GRAPHICS",		sf::Color::White},
		{sf::Vector2f{ -10, -150 },		buttonSize,		sf::Color::Transparent,			 40, "RESOLUTION",		sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ -10, -50 },		buttonSize,		sf::Color::Transparent,			 40, "FPS LIMIT",		sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ -10, 50 },		buttonSize,		sf::Color::Transparent,			 40, "VSYNC",			sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ -10, 150 },		buttonSize,		sf::Color::Transparent,			 40, "MENU PARALLAX",	sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 10, 300 },		buttonSize,		sf::Color::White,				 24, "APPLY",			sf::Color::Black, EAlignment::LEFT},
		{sf::Vector2f{ -10, 300 },		buttonSize,		sf::Color::White,				 24, "RETURN",			sf::Color::Black, EAlignment::RIGHT}

	};

	optionsGraphics_test.construct(MAIN_MENU_CONSTR[0]);
	optionsGraphics_Resolution.construct(MAIN_MENU_CONSTR[1]);
	optionsGraphics_FPSLimit.construct(MAIN_MENU_CONSTR[2]);
	optionsGraphics_Vsync.construct(MAIN_MENU_CONSTR[3]);
	optionsGraphics_MenuParallax.construct(MAIN_MENU_CONSTR[4]);
	optionsGraphics_Apply.construct(MAIN_MENU_CONSTR[5]);
	optionsGraphics_return.construct(MAIN_MENU_CONSTR[6]);
}

void W_OptionsGraphics::construct()
{
	InputWidget::construct();
	shapes = { &optionsGraphics_test, &optionsGraphics_return,&optionsGraphics_Resolution, &optionsGraphics_FPSLimit,
				&optionsGraphics_Vsync, &optionsGraphics_MenuParallax, &optionsGraphics_Apply };
}

void W_OptionsGraphics::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	optionsGraphics_test.tick(deltaTime);
	optionsGraphics_return.tick(deltaTime);
	optionsGraphics_Apply.tick(deltaTime);
	optionsGraphics_Resolution.tick(deltaTime);
	optionsGraphics_FPSLimit.tick(deltaTime);
	optionsGraphics_Vsync.tick(deltaTime);
	optionsGraphics_MenuParallax.tick(deltaTime);
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