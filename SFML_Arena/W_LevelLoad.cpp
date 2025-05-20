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

	tick_textAnim(deltaTime);

	bg.tick(deltaTime);
	T_Title.tick(deltaTime);
	fadeScreen.tick(deltaTime);
}

void W_LevelLoad::start_textAnim()
{
	static constexpr float DURATION = 3.0f;

	sf::Color t_color;

	switch (textAnimPhase)
	{
	case 0:
		t_color = textFade.reset(ColorColor(sf::Color::Transparent, sf::Color::Red), DURATION / 3.0f, easing::expo::out);
		T_Title.setTextColor(t_color);
		break;
	case 1:
		t_color = textFade.reset(ColorColor(sf::Color::Red, sf::Color::White), DURATION / 3.0f, easing::expo::out);
		T_Title.setTextColor(t_color);
		break;
	case 2:
		t_color = textFade.reset(ColorColor(sf::Color::White, sf::Color::Red), DURATION / 3.0f, easing::smoothstep);
		T_Title.setTextColor(t_color);
		break;
	default:
		break;
	}
}

void W_LevelLoad::tick_textAnim(const float& deltaTime)
{
	T_Title.setTextColor(textFade.fade(deltaTime));

	if (textFade.inProgress())
		return;
	
	textAnimPhase++;
	start_textAnim();
}

void W_LevelLoad::start_openAnim()
{
	textAnimPhase = 0;
	start_textAnim();

	static constexpr float DURATION = 1.0f;
	
	fadeScreen.reset(ColorColor(sf::Color::Transparent, sf::Color::Transparent), DURATION, easing::expo::in);
}

void W_LevelLoad::tick_openAnim(const float& deltaTime)
{
	fadeScreen.fade(deltaTime);

	if (fadeScreen.isFading())
		return;

	stopAnim(OPEN_ANIM);
	playAnim(CLOSE_ANIM);
}

void W_LevelLoad::start_closeAnim()
{
	static constexpr float DURATION = 2.0f;

	fadeScreen.reset(ColorColor(sf::Color::Transparent, sf::Color::Black), DURATION, easing::smootherstep);
}

void W_LevelLoad::tick_closeAnim(const float& deltaTime)
{
	fadeScreen.fade(deltaTime);

	if (fadeScreen.isFading())
		return;

	stopAnim(CLOSE_ANIM);
	// Play opening Animation in master Gameplay Widget
	parent->playAnim(OPEN_ANIM);
}