#pragma once

#include "WidgetElements.h"

struct ColorColor
{
	sf::Color color1;
	sf::Color color2;
};

class ColorFade : public Button
{
private:
	bool bStopFade = true;

	ColorColor boxFade = ColorColor(sf::Color::Black, sf::Color::Transparent);
	ColorColor textFade = ColorColor(sf::Color::White, sf::Color::Transparent);
	float duration = 1.0f;
	float elapsedTime = 0.0f;

	void tick_fade(const float& deltaTime);

	EasingFunction easingFunc = easing::smootherstep; // Or default to smootherstep adapter

public:
	ColorFade(InputWidget* parent);
	void tick(const float& deltaTime) override;

	void reset() override;

	void setFadeColor(const ColorColor& boxFade, const ColorColor& textFade, const float& d, EasingFunction easing);
	void setFadeColor(const ColorColor& bothFade, const float& d, EasingFunction easing);
	void setFadeColor(const ColorColor& bothFade, const float& d);
	bool isFading() const;

	void startFade();
	void stopFade();
};