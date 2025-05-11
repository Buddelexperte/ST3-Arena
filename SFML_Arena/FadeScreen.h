#pragma once

#include "Border.h" // Screen

class FadeScreen : public WidgetElement
{
private:
	sf::Vector2f getTickCorrection() const override;
	Border screen;
	ColorFade colorFade;

public:
	FadeScreen(InputWidget* parent);
	void construct() override;
	void tick(const float& deltaTime) override;

	void fade(const float&);

	void reset(const ColorColor& bothFade, const float& d, EasingFunction easing);
	void reset(const ColorColor& bothFade, const float& d);
	bool isFading() const;
	bool isDone() const;

};