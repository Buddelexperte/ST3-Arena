#pragma once

#include "LoadingScreen.h" // Own header file

#include "GameInstance.h"


W_LoadingScreen::W_LoadingScreen(InputWidget* parent)
	: InputWidget(parent),
	fadeScreen(this), T_LoadingScreenStatus(this), T_GameTitle(this), textChangeTimer(TEXT_CHANGE_DELAY)
{
	const std::vector<RawButton> CONSTR = {
		{viewTL, viewSize, sf::Color::Transparent, 50, "LOADING...", sf::Color::White, EAlignment::CENTER, EAlignment::CENTER_BOTTOM},
		{viewTL, viewSize, sf::Color::Transparent, 500, "ARENA", sf::Color::White, EAlignment::CENTER, EAlignment::CENTER},
		{viewTL, viewSize}
	};

	T_LoadingScreenStatus.construct(CONSTR[0]);
	T_GameTitle.construct(CONSTR[1]);
	fadeScreen.construct(CONSTR[2]);

	shapes = { &T_LoadingScreenStatus, &T_GameTitle, &fadeScreen };

	std::cout << "- Constructed Loading Screen" << std::endl;
}

void W_LoadingScreen::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
	T_LoadingScreenStatus.tick(deltaTime);
	T_GameTitle.tick(deltaTime);
	fadeScreen.tick(deltaTime);
}

void W_LoadingScreen::start_openAnim()
{
	fadeScreen.setFadeColor(sf::Color::Black, sf::Color::Transparent, SCREEN_FADE_DURATION);
	fadeScreen.startFade();
}

void W_LoadingScreen::tick_openAnim(const float& deltaTime)
{
	if (!fadeScreen.isFading())
	{
		textChangeTimer.addValue(-deltaTime);

		if (textChangeTimer.isEmpty())
		{
			textChangeTimer.reset();

			textState++;

			if (textState >= 3)
			{
				if (getPlayingAnim() != CLOSE_ANIM)
					playAnim(CLOSE_ANIM);
			}

			std::string newMessage;
			switch (textState % 4)
			{
			case 0:
				gameInstance().modWindowName("");
				newMessage = "LOADING...";
				break;
			case 1:
				gameInstance().modWindowName("...");
				newMessage = ".LOADING..";
				break;
			case 2:
				gameInstance().modWindowName("..");
				newMessage = "..LOADING.";
				break;
			case 3:
			default:
				gameInstance().modWindowName(".");
				newMessage = "...LOADING";
				break;
			}

			T_LoadingScreenStatus.setText(newMessage);
		}
	}
}

void W_LoadingScreen::start_closeAnim()
{
	fadeScreen.setFadeColor(sf::Color::Transparent, sf::Color::Black, SCREEN_FADE_DURATION, easing::cubic::out);
	fadeScreen.startFade();
}

void W_LoadingScreen::tick_closeAnim(const float&)
{
	if (!fadeScreen.isFading())
	{
		gameInstance().setGameState(MENU_SCREEN);
		IWidgetAnimation::stopAnim();
	}
}
