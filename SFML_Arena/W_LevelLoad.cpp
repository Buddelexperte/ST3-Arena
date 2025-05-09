#pragma once

#include "W_LevelLoad.h"

sf::Vector2f W_LevelLoad::getTickCorrection() const
{
	return viewCenter;
}

W_LevelLoad::W_LevelLoad(InputWidget* parent)
	: InputWidget(parent),
	bg(this), T_Title(this), fadeScreen(this)
{
	static const RawButton CONSTR = {
		viewTL, buttonSize, sf::Color::Transparent, 500, "ARENA", sf::Color::Transparent, EAlignment::CENTER, EAlignment::CENTER
	};

	bg.reset(ColorColor(sf::Color::Black, sf::Color::Black), 0.0f);
	T_Title.construct(CONSTR, NOT_INTERACTABLE_FLAG);

	shapes = { &bg, &T_Title, &fadeScreen };
}

void W_LevelLoad::construct()
{
	InputWidget::construct();

	bg.construct();
	T_Title.construct();
	fadeScreen.construct();
}

void W_LevelLoad::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	bg.tick(deltaTime);
	T_Title.tick(deltaTime);
	fadeScreen.tick(deltaTime);
}

void W_LevelLoad::start_openAnim()
{
	static constexpr float DURATION = 1.0f;
	
	sf::Color t_color = textFade.reset(ColorColor(sf::Color::Transparent, sf::Color::White), DURATION / 3.0f, easing::quad::in);
	T_Title.setColor(t_color, true);
	fadeScreen.reset(ColorColor(sf::Color::Transparent, sf::Color::Transparent), DURATION, easing::expo::in);
}

void W_LevelLoad::tick_openAnim(const float& deltaTime)
{
	T_Title.setColor(textFade.fade(deltaTime), true);
	fadeScreen.fade(deltaTime);

	if (fadeScreen.isFading())
		return;

	stopAnim(OPEN_ANIM);
	playAnim(CLOSE_ANIM);
}

void W_LevelLoad::start_closeAnim()
{
	static constexpr float DURATION = 2.0f;

	sf::Color t_color = textFade.reset(ColorColor(sf::Color::White, sf::Color::Transparent), DURATION, easing::smoothstep);
	T_Title.setColor(t_color, true);
	fadeScreen.reset(ColorColor(sf::Color::Transparent, sf::Color::Black), DURATION, easing::smootherstep);
}

void W_LevelLoad::tick_closeAnim(const float& deltaTime)
{
	T_Title.setColor(textFade.fade(deltaTime), true);
	fadeScreen.fade(deltaTime);

	if (fadeScreen.isFading())
		return;

	stopAnim(CLOSE_ANIM);
	// Play opening Animation in master Gameplay Widget
	parent->playAnim(OPEN_ANIM);
}