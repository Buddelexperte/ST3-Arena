#pragma once

#include "WidgetElements.h"

class ColorFade : public Button
{
private:
	bool bStopFade = true;

	sf::Color fromColor = sf::Color::Black;
	sf::Color toColor = sf::Color::Transparent;
	float duration = 1.0f;
	float elapsedTime = 0.0f;

	void tick_fade(const float& deltaTime);

	EasingFunction easingFunc = easing::smootherstep; // Or default to smootherstep adapter

public:
	ColorFade(InputWidget* parent);
	void tick(const float& deltaTime) override;

	void reset() override;

	void setFadeColor(const sf::Color& from, const sf::Color& to, const float& d, EasingFunction easing);
	void setFadeColor(const sf::Color& from, const sf::Color& to, const float& d);
	bool isFading() const;

	void startFade();
	void stopFade();
};