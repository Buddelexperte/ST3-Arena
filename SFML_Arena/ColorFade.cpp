#pragma once

#include "ColorFade.h" // Own header file
#include "Functions.h"

void ColorFade::tick_fade(const float& deltaTime)
{
	if (bStopFade || !easingFunc)
		return;

	elapsedTime += deltaTime;
	float t = std::min(elapsedTime, duration);

	// Call the selected easing function
	float factor = easingFunc(t, 0.0f, 1.0f, duration);

	// Lerp and apply new color
	sf::Color newColor = lerp(boxFade.color1, boxFade.color2, factor);
	setColor(newColor, false);

	newColor = lerp(textFade.color1, textFade.color2, factor);
	setColor(newColor, true);

	if (factor >= 1.0f)
		bStopFade = true;
}

ColorFade::ColorFade(InputWidget* parent)
	: Button(parent)
{
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
	setColor(boxFade.color1, false);
	setColor(textFade.color1, true);
}


// Fade both, using default easing
void ColorFade::setFadeColor(const ColorColor& bothFade, const float& d)
{
	setFadeColor(bothFade, d, easing::smootherstep);
}

// Fade both, using param easing
void ColorFade::setFadeColor(const ColorColor& bothFade, const float& d, EasingFunction easing)
{
	setFadeColor(bothFade, bothFade,  d, easing);
}

// Box and Text seperated colorways, easing in param
void ColorFade::setFadeColor(const ColorColor& b_fade, const ColorColor& t_fade, const float& d, EasingFunction easing)
{
	boxFade = b_fade;
	textFade = t_fade;
	duration = d;
	easingFunc = easing;
}


bool ColorFade::isFading() const
{
	if (bStopFade)
		return false;

	return (elapsedTime < duration);
}

void ColorFade::startFade()
{
	bStopFade = false;
	elapsedTime = 0.0f;

	setColor(boxFade.color1, false);
	setColor(textFade.color1, true);
}

void ColorFade::stopFade()
{
	bStopFade = true;
}
