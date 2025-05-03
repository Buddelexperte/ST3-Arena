#pragma once

#include "LoadingScreen.h" // Own header file

#include "GameInstance.h"

void W_LoadingScreen::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
	T_LoadingScreenStatus.tick(deltaTime);
	fadeScreen.tick(deltaTime);
}

W_LoadingScreen::W_LoadingScreen(InputWidget* parent)
	: InputWidget(parent),
	fadeScreen(this), T_LoadingScreenStatus(this), textChangeTimer(TEXT_CHANGE_DELAY)
{
	const std::vector<RawButton> CONSTR = {
		{viewTL, viewSize, sf::Color::Black, 50, "LOADING...", sf::Color::White, EAlignment::CENTER, EAlignment::CENTER_BOTTOM}
	};

	T_LoadingScreenStatus.construct(CONSTR[0]);

	fadeScreen.setPosition(viewTL);
	fadeScreen.setSize(viewSize);

	shapes = { &T_LoadingScreenStatus, &fadeScreen };

	std::cout << "- Constructed Loading Screen" << std::endl;
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
				newMessage = "LOADING...";
				break;
			case 1:
				newMessage = ".LOADING..";
				break;
			case 2:
				newMessage = "..LOADING.";
				break;
			case 3:
			default:
				newMessage = "...LOADING";
				break;
			}

			T_LoadingScreenStatus.setText(newMessage);
		}
	}
}

void W_LoadingScreen::start_closeAnim()
{
	fadeScreen.setFadeColor(sf::Color::Transparent, sf::Color::Black, SCREEN_FADE_DURATION);
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
