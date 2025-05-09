#pragma once

#include "LoadingScreen.h" // Own header file

#include "GameInstance.h"


W_LoadingScreen::W_LoadingScreen(InputWidget* parent)
	: InputWidget(parent),
	fadeScreen(this), T_LoadingScreenStatus(this), T_GameTitle(this)
{
	const std::vector<RawButton> CONSTR = {
		{viewTL, viewSize, sf::Color::Transparent, 50, "LOADING...", sf::Color::White, EAlignment::CENTER, EAlignment::CENTER_BOTTOM},
		{viewTL, viewSize, sf::Color::Transparent, 500, "ARENA", sf::Color::White, EAlignment::CENTER, EAlignment::CENTER}
	};

	T_LoadingScreenStatus.construct(CONSTR[0], NOT_INTERACTABLE_FLAG);
	T_GameTitle.construct(CONSTR[1], NOT_INTERACTABLE_FLAG);

	shapes = { &T_LoadingScreenStatus, &T_GameTitle, &fadeScreen };

	std::cout << "- Constructed Loading Screen" << std::endl;
}

void W_LoadingScreen::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_LoadingScreenStatus.tick(deltaTime);
	T_GameTitle.tick(deltaTime);
}

void W_LoadingScreen::updateStatus(const unsigned int stage = 0)
{
	std::string newMessage = "STATUS_PLACEHOLDER";
	switch (stage % 4)
	{
	case 0:
		gameInstance().modWindowName("...");
		newMessage = "LOADING...";
		break;
	case 1:
		gameInstance().modWindowName("..");
		newMessage = ".LOADING..";
		break;
	case 2:
		gameInstance().modWindowName(".");
		newMessage = "..LOADING.";
		break;
	case 3:
	default:
		gameInstance().modWindowName("");
		newMessage = "...LOADING";
		break;
	}

	T_LoadingScreenStatus.setText(newMessage);
}

void W_LoadingScreen::start_openAnim()
{
	fadeScreen.reset(ColorColor(sf::Color::Black, sf::Color::Transparent), SCREEN_FADE_DURATION);
	playAnim(IDLE_ANIM);
}

void W_LoadingScreen::tick_openAnim(const float& deltaTime)
{
	fadeScreen.fade(deltaTime);

	if (fadeScreen.isFading())
		return;

	stopAnim(OPEN_ANIM);
}

void W_LoadingScreen::start_idleAnim()
{
	textState = -1;
	gameInstance().modWindowName("...");
	textChangeTimer.setMaxValue(TEXT_CHANGE_DELAY);
	textChangeTimer.fill_to_max();
}

void W_LoadingScreen::tick_idleAnim(const float& deltaTime)
{
	// Wait for timer to run out
	textChangeTimer.addValue(-deltaTime);
	if (textChangeTimer.isNotEmpty())
		return;

	textChangeTimer.fill_to_max();
	
	textState++;
	
	updateStatus(textState);
	
	if (textState >= 3)
	{
		stopAnim(IDLE_ANIM);
		
		if (!isAnimPlaying(CLOSE_ANIM))
			playAnim(CLOSE_ANIM);
	}
}

void W_LoadingScreen::start_closeAnim()
{
	fadeScreen.reset(ColorColor(sf::Color::Transparent, sf::Color::Black), TEXT_CHANGE_DELAY);
}

void W_LoadingScreen::tick_closeAnim(const float& deltaTime)
{
	fadeScreen.fade(deltaTime);
	
	if (fadeScreen.isFading())
		return;
	
	gameInstance().setGameState(MENU_SCREEN);
	stopAnim(CLOSE_ANIM);
}
