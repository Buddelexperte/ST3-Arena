#pragma once

#include "W_OptionsSounds.h" // Own header file

#include "GameInstance.h"
#include "SoundManager.h"

W_OptionsSounds::W_OptionsSounds(InputWidget* parent = nullptr) 
	: InputWidget(parent),
	T_Title(this), B_Return(this), B_ToggleSound(this)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{sf::Vector2f{ 0, -300 },   sf::Vector2f{ 350, 120 }, sf::Color::Transparent,	100,	"SOUND",		sf::Color::White},
		{sf::Vector2f{ 0, 300 },	sf::Vector2f{ 300, 100 }, sf::Color::White,			24,		"RETURN",	sf::Color::Black},
		{sf::Vector2f{ 0, 0 },	sf::Vector2f{ 300, 100 }, sf::Color::White,			24,		"SOUND -> AUS",	sf::Color::Black}
	};

	T_Title.construct(MAIN_MENU_CONSTR[0]);
	B_Return.construct(MAIN_MENU_CONSTR[1]);
	B_ToggleSound.construct(MAIN_MENU_CONSTR[2]);
}

void W_OptionsSounds::construct()
{
	InputWidget::construct();
	shapes = { &T_Title, &B_Return, &B_ToggleSound };
}

void W_OptionsSounds::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);
	B_Return.tick(deltaTime);
	B_ToggleSound.tick(deltaTime);
}

bool W_OptionsSounds::isMouseOver(const bool& checkForClick = false)
{
	if (B_Return.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->construct();
		return true;
	}

	if (B_ToggleSound.isMouseOver(checkForClick))
	{
		if (checkForClick) toggleMuteSound();
		return true;
	}

	// On no button-mouse overlap
	return false;
}

void W_OptionsSounds::toggleMuteSound()
{
	SoundManager& sm = SoundManager::getInstance();
	if (sm.getMasterVolume() == 0)
	{
		sm.setMasterVolume(savedVolume);
		sm.play(sm.getSound_Click(), ESoundEnv::UI);
		B_ToggleSound.setText("SOUND -> AUS");
	}
	else
	{
		savedVolume = sm.getMasterVolume();
		sm.setMasterVolume(0);
		B_ToggleSound.setText("SOUND -> AN");
	}
}