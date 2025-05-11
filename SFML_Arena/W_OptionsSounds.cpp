#pragma once

#include "W_OptionsSounds.h" // Own header file

#include "GameInstance.h"
#include "SoundManager.h"

W_OptionsSounds::W_OptionsSounds(InputWidget* parent) 
	: InputWidget(parent),
	T_Title(this), B_Return(this), B_ToggleSound(this)
{
	const RawText TITLE_CONSTR = {
		sf::Vector2f{ 0, -300 }, sf::Color::White, 100, "SOUND", EAlignment::CENTER
	};

	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{sf::Vector2f{ 0, 300 },	sf::Vector2f{ 300, 100 },	sf::Color::White,			24,		"RETURN",		sf::Color::Black},
		{sf::Vector2f{ 0, 0 },		sf::Vector2f{ 300, 100 },	sf::Color::White,			24,		"SOUND VALUE",	sf::Color::Black}
	};

	T_Title.construct(TITLE_CONSTR);
	B_Return.construct(MAIN_MENU_CONSTR[0]);
	B_ToggleSound.construct(MAIN_MENU_CONSTR[1]);

	delegateButtons();

	shapes = { &T_Title, &B_Return, &B_ToggleSound };
}

void W_OptionsSounds::construct()
{
	InputWidget::construct();

	updateText();

	T_Title.construct();
	B_Return.construct();
	B_ToggleSound.construct();
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
		return true;

	if (B_ToggleSound.isMouseOver(checkForClick))
		return true;

	// On no button-mouse overlap
	return false;
}

void W_OptionsSounds::delegateButtons()
{
	B_Return.onClick = [this]() {
		onKeyEscape();
		};

	B_ToggleSound.onClick = [this]() {
		toggleMuteSound();
		};
}

void W_OptionsSounds::call_muteSound(const bool bMute)
{
	SoundManager& sm = SoundManager::getInstance();
	sm.setIsMuted(bMute, ESoundEnv::MASTER);

	if (bMute)
		return;

	sm.play(sm.getSound_Click(), ESoundEnv::UI);
	
}

void W_OptionsSounds::toggleMuteSound()
{
	SoundManager& sm = SoundManager::getInstance();

	bool isMuted = sm.getIsMuted();
	bool shouldMute = (!isMuted);
	call_muteSound(shouldMute);

	updateText();
}

void W_OptionsSounds::updateText()
{
	SoundManager& sm = SoundManager::getInstance();
	bool bIsMuted = sm.getIsMuted(ESoundEnv::MASTER);

	if (bIsMuted)
	{
		B_ToggleSound.setText("SOUND: STUMM");
		return;
	}
	std::string volumeAsString = std::to_string(static_cast<int>(sm.getMasterVolume()));
	B_ToggleSound.setText("SOUND: " + volumeAsString + "%");
}
