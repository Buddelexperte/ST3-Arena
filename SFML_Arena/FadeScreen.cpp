#pragma once

#include "FadeScreen.h" // Own header file

void FadeScreen::fade(const float& deltaTime)
{
	if (colorFade.done())
		return;

	screen.setColor(colorFade.fade(deltaTime));
}

sf::Vector2f FadeScreen::getTickCorrection() const
{
	return viewCenter;
}

FadeScreen::FadeScreen(InputWidget* parent)
	: WidgetElement(parent), screen(parent)
{
	static const RawBorder CONSTR = {
		viewTL, viewSize, sf::Color::Transparent
	};

	screen.construct(CONSTR);

	shapes = { &screen };
}

void FadeScreen::construct()
{
	WidgetElement::construct();

	screen.construct();
}

void FadeScreen::tick(const float& deltaTime)
{
	WidgetElement::tick(deltaTime);

	screen.tick(deltaTime);
}

// Fade both, using default easing
void FadeScreen::reset(const ColorColor& fade, const float& duration)
{
	reset(fade, duration, easing::smootherstep);
}

// Box and Text seperated colorways, easing in param
void FadeScreen::reset(const ColorColor& fade, const float& duration, EasingFunction easing)
{
	colorFade.reset(fade, duration, easing);
	screen.setColor(colorFade.current);
}


bool FadeScreen::isFading() const
{
	return (colorFade.elapsedTime < colorFade.d);
}

bool FadeScreen::isDone() const
{
	return colorFade.done();
}
