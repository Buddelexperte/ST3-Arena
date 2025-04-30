#pragma once

#include "W_Hud.h" // Own header file
#include "GameInstance.h"

// Important for HUD display on screen
sf::Vector2f W_Hud::getCorrectTickCorrection() const
{
	return IDrawableShapes::viewTL;
}

W_Hud::W_Hud(InputWidget* parent)
	: InputWidget(parent),
	healthBar(this), scoreBar(this), fadeScreen(this)
{
	// Setting fade from black to transparent for spawn visuals
	fadeScreen.setFadeColor(sf::Color::Black, sf::Color::Transparent, SCREEN_FADE_DURATION);
	fadeScreen.setPosition(viewCenter);
	fadeScreen.setSize(viewSize);
}

void W_Hud::construct()
{
	setWidgetIndex(0);

	fadeScreen.startFade();

	healthBar.construct();
	scoreBar.construct();
}

void W_Hud::reset()
{
	reset_health();
	reset_score();
}

void W_Hud::reset_health()
{
	healthBar.reset();

}

void W_Hud::reset_score()
{
	scoreBar.reset();
}

void W_Hud::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	healthBar.tick(deltaTime);
	scoreBar.tick(deltaTime);
	fadeScreen.tick(deltaTime);
}

InputWidget* W_Hud::setWidgetIndex(const int& newIndex)
{
	// Default drawables used
	shapes = { &healthBar, &scoreBar, &fadeScreen };

	switch (widgetIndex = newIndex)
	{
	case 0: // SELF
		break;
	default:
		shapes = { };
		break;
	}

	return getActiveChild();
}

InputWidget* W_Hud::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0:
		return this;
		break;
	default:
		break;
	}

	return nullptr;
}