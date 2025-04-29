#pragma once

#include "ColorFade.h" // Own header file
#include "Functions.h"

void ColorFade::tick_fade(const float& deltaTime)
{
	if (bStopFade)
		return;

	const sf::Color currentColor = getColor();
	
	if (currentColor == toColor)
	{
		bStopFade = true;
		return;
	}

	float factor = deltaTime / fadeTime;
	sf::Color newColor = lerp(currentColor, toColor, factor);
	setColor(newColor);
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
	bool bFinished = (getColor() == toColor);

	return (!bFinished);
}

void ColorFade::startFade()
{
	bStopFade = false;

	setColor(fromColor);
}

void ColorFade::stopFade()
{
	bStopFade = true;
}
