#pragma once

#include "ColorFade.h" // Own header file
#include "Functions.h"

void ColorFade::tick_fade(const float& deltaTime)
{
	if (bStopFade)
		return;

	elapsedTime += deltaTime;

	float factor = std::clamp(elapsedTime / fadeTime, 0.0f, 1.0f);

	sf::Color newColor = lerp(fromColor, toColor, factor);
	setColor(newColor);

	sf::Color newTextColor = lerp(fromColor, toColor, factor);
	setColor(newTextColor, true);

	if (factor >= 1.0f)
		bStopFade = true;
}


ColorFade::ColorFade(InputWidget* parent)
	: Button(parent)
{
	// Set default values
	fromColor = sf::Color::Black;
	toColor = sf::Color::Transparent;
	fadeTime = 1.0f;
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
}


// From, to and time
void ColorFade::setFadeColor(const sf::Color& from, const sf::Color& to, const float& time)
{
	fromColor = from;
	toColor = to;
	fadeTime = time;
}

bool ColorFade::isFading() const
{
	if (bStopFade)
		return false;

	//bool bFinished = (getColor() == toColor);
	bool bFinished = (elapsedTime >= fadeTime);

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
