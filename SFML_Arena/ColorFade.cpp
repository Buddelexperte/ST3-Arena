#pragma once

#include "ColorFade.h" // Own header file
#include "Functions.h"

void Button_fancy::tick_fade(const float& deltaTime)
{
	if (bStopFade || !easingFunc)
		return;

	elapsedTime += deltaTime;
	float t = std::min(elapsedTime, duration);

	// Call the selected easing function
	float factor = easingFunc(t, 0.0f, 1.0f, duration);

	// Lerp and apply new color
	sf::Color newColor = lerp(boxFade.color0, boxFade.color1, factor);
	setColor(newColor, false);

	newColor = lerp(textFade.color0, textFade.color1, factor);
	setColor(newColor, true);

	if (factor >= 1.0f)
		bStopFade = true;
}

Button_fancy::Button_fancy(InputWidget* parent)
	: Button(parent)
{
}

void Button_fancy::tick(const float& deltaTime)
{
	Button::tick(deltaTime);

	tick_fade(deltaTime);
}

void Button_fancy::reset()
{
	Button::reset();

	bStopFade = true;
	elapsedTime = 0.0f;

	// Reset fade screen to default color
	setColor(boxFade.color0, false);
	setColor(textFade.color0, true);
}


// Fade both, using default easing
void Button_fancy::setFadeColor(const ColorColor& bothFade, const float& d)
{
	setFadeColor(bothFade, d, easing::smootherstep);
}

// Fade both, using param easing
void Button_fancy::setFadeColor(const ColorColor& bothFade, const float& d, EasingFunction easing)
{
	setFadeColor(bothFade, bothFade,  d, easing);
}

// Box and Text seperated colorways, easing in param
void Button_fancy::setFadeColor(const ColorColor& b_fade, const ColorColor& t_fade, const float& d, EasingFunction easing)
{
	boxFade = b_fade;
	textFade = t_fade;
	duration = d;
	easingFunc = easing;
}


bool Button_fancy::isFading() const
{
	if (bStopFade)
		return false;

	return (elapsedTime < duration);
}

void Button_fancy::startFade()
{
	bStopFade = false;
	elapsedTime = 0.0f;

	setColor(boxFade.color0, false);
	setColor(textFade.color0, true);
}

void Button_fancy::stopFade()
{
	bStopFade = true;
}
