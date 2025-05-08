#pragma once

#include "Functions.h"

struct ColorColor
{
	sf::Color color0;
	sf::Color color1;
};

struct ColorFade
{
	sf::Color current			= sf::Color::Transparent;										// Actual color, should be used
	ColorColor fromTo			= ColorColor(sf::Color::Transparent, sf::Color::Transparent);	// Color to fade from and to
	float d						= 0.0f;															// Duration
	float elapsedTime			= 0.0f;															// Time ticked since last reset
	EasingFunction easingFunc	= easing::smootherstep;											// Easing function used on time t

	void reset()
	{
		current = fromTo.color0;
		elapsedTime = 0.0f;
	}

	void reset(const ColorColor& newPair, const float duration, EasingFunction e = easing::smootherstep)
	{
		fromTo = newPair;
		d = duration;
		easingFunc = e;
		reset();
	}


	sf::Color fade(const float& deltaTime)
	{
		if (!easingFunc)
			return current;

		// Update elapsed time and clamp t (time) between 0 and d (duration)
		elapsedTime += deltaTime;
		float t = std::min(elapsedTime, d);

		// Call the selected easing function
		float factor = easingFunc(t, 0.0f, 1.0f, d);

		// Lerp and apply new color
		return (current = lerp(fromTo.color0, fromTo.color1, factor));
	}

	bool done() const
	{
		return (elapsedTime >= d);
	}

	void blockFade()
	{
		fromTo.color1 = fromTo.color0;
		d = 0.0f;
	}
};