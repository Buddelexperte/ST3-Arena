#pragma once

#include "LoadingScreen.h" // Own header file

#include "GameInstance.h"

void W_LoadingScreen::tick(const float& deltaTime)
{
	textChangeTimer.addValue(-deltaTime);

	fadeScreen.tick(deltaTime);

	if (textChangeTimer.isEmpty())
	{
		textChangeTimer.reset();

		textState++;

		if (textState == 7)
		{
			fadeScreen.startFade();

			bLoadedIn = true;
		}

		std::string newMessage;
		switch (textState % 4)
		{
		case 0:
			newMessage = "LOADING...";
			break;
		case 1:
			newMessage = "LOADING..";
			break;
		case 2:
			newMessage = "LOADING.";
			break;
		case 3:
		default:
			newMessage = "LOADING";
			break;
		}

		fadeScreen.setText(newMessage);
	}

	if (bLoadedIn && !fadeScreen.isFading())
	{
		gameInstance().setGameState(MENU_SCREEN);
	}
}

W_LoadingScreen::W_LoadingScreen(InputWidget* parent)
	: InputWidget(parent),
	fadeScreen(this), textChangeTimer(TEXT_CHANGE_DELAY)
{
	const RawButton CONSTR = {viewTL, viewSize, sf::Color::Black, 50, "LOADING...", sf::Color::White, EAlignment::CENTER, EAlignment::CENTER_BOTTOM};

	fadeScreen.construct(CONSTR);

	fadeScreen.setFadeColor(sf::Color::Black, sf::Color::Transparent, SCREEN_FADE_DURATION);

	shapes = { &fadeScreen };

	std::cout << "- Constructed Loading Screen" << std::endl;
}