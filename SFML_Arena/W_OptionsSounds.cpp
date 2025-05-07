#pragma once

#include "W_OptionsSounds.h" // Own header file

#include "GameInstance.h"
#include "SoundManager.h"

W_OptionsSounds::W_OptionsSounds(InputWidget* parent = nullptr) 
	: InputWidget(parent),
	optionsSounds_test(this), optionsSounds_return(this), optionsSounds_soundAus(this)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{sf::Vector2f{ 0, -300 },   sf::Vector2f{ 350, 120 }, sf::Color::Transparent,	100,	"SOUND",		sf::Color::White},
		{sf::Vector2f{ 0, 300 },	sf::Vector2f{ 300, 100 }, sf::Color::White,			24,		"RETURN",	sf::Color::Black},
		{sf::Vector2f{ 0, 0 },	sf::Vector2f{ 300, 100 }, sf::Color::White,			24,		"SOUND AUS",	sf::Color::Black}
	};

	optionsSounds_test.construct(MAIN_MENU_CONSTR[0]);
	optionsSounds_return.construct(MAIN_MENU_CONSTR[1]);
	optionsSounds_soundAus.construct(MAIN_MENU_CONSTR[2]);
}

void W_OptionsSounds::construct()
{
	InputWidget::construct();
	shapes = { &optionsSounds_test, &optionsSounds_return, &optionsSounds_soundAus };
}

void W_OptionsSounds::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	optionsSounds_test.tick(deltaTime);
	optionsSounds_return.tick(deltaTime);
	optionsSounds_soundAus.tick(deltaTime);
}

bool W_OptionsSounds::isMouseOver(const bool& checkForClick = false)
{
	if (optionsSounds_return.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->construct();
		return true;
	}

	if (optionsSounds_soundAus.isMouseOver(checkForClick))
	{
		if (checkForClick) soundAus();
		return true;
	}

	// On no button-mouse overlap
	return false;
}

void W_OptionsSounds::soundAus()
{
	SoundManager::getInstance().setMasterVolume(0);
}