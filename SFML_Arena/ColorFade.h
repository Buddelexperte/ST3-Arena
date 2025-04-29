#pragma once

#include "WidgetElements.h"

class ColorFade : public Button
{
private:
	bool bStopFade = true;

	sf::Color fromColor;
	sf::Color toColor;
	float fadeTime;

	void tick_fade(const float& deltaTime);

public:
	ColorFade(InputWidget* parent);
	void tick(const float& deltaTime) override;

	void reset() override;

	void setFadeColor(const sf::Color& from, const sf::Color& to, const float& time);
	bool isFading() const;

	void startFade();
	void stopFade();
};