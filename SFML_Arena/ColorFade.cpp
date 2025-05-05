#pragma once

#include "ColorFade.h" // Own header file
#include "Functions.h"

void ColorFade::tick_fade(const float& deltaTime)
{
	if (bStopFade)
		return;

	elapsedTime += deltaTime;

	float t = std::min(elapsedTime, duration);
	float factor = easing::smootherstep(0.0f, 1.0f, elapsedTime / duration);
	factor = easing::cubic::inOut(t, 0.0f, 1.0f, duration);

	sf::Color newColor = lerp(fromColor, toColor, factor);
	setColor(newColor);

	newColor = lerp(getColor(true), toColor, factor);
	T_Text.setFillColor(newColor);
	T_Text.setOutlineColor(newColor);

	if (factor >= 1.0f)
		bStopFade = true;
}


ColorFade::ColorFade(InputWidget* parent)
	: Button(parent)
{
	// Set default values
	fromColor = sf::Color::Black;
	toColor = sf::Color::Transparent;
	duration = 1.0f;
}

void ColorFade::tick(const float& deltaTime)
{
	Button::tick(deltaTime);

	tick_fade(deltaTime);
}

void ColorFade::reset()
{
	Button::reset();

	bStopFade = true;
	elapsedTime = 0.0f;

	// Reset fade screen to default color
	setColor(fromColor);
	setColor(getColor(true));
}


// From, to and time
void ColorFade::setFadeColor(const sf::Color& from, const sf::Color& to, const float& time)
{
	fromColor = from;
	toColor = to;
	duration = time;
}

bool ColorFade::isFading() const
{
	if (bStopFade)
		return false;

	bool bFinished = (elapsedTime >= duration);

	return (!bFinished);
}

void ColorFade::startFade()
{
	bStopFade = false;
	elapsedTime = 0.0f;

	setColor(fromColor);
	setColor(getButtonData().textColor, true);
}

void ColorFade::stopFade()
{
	bStopFade = true;
}
