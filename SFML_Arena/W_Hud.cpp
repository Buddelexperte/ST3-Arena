#pragma once

#include "W_Hud.h" // Own header file
#include "GameInstance.h"


W_Hud::W_Hud(InputWidget* parent)
	: InputWidget(parent)
{
	const std::vector<RawButton> HUD_CONSTR = {
		{viewTL + sf::Vector2f{ 0.0f, 0.0f },			sf::Vector2f{viewSize.x, 70.0f},	sf::Color::Green,	70, "100",	sf::Color::Black, EAlignment::LEFT_TOP, EAlignment::LEFT},
		{viewTL + sf::Vector2f{ 0.0f, 0.0f },			sf::Vector2f{viewSize.x, 70.0f},	sf::Color::Red,		0, "",	sf::Color::Transparent, EAlignment::LEFT_TOP, EAlignment::LEFT}
	};

	lifeBar.construct(HUD_CONSTR[0]);
	lifeBar_bg.construct(HUD_CONSTR[1]);
}

void W_Hud::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);
}

void W_Hud::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
	lifeBar.setPos(viewTL		+ sf::Vector2(0.0f, 0.0f));
	lifeBar_bg.setPos(viewTL	+ sf::Vector2(0.0f, 0.0f));

	tick_lifeBar(deltaTime);
}

void W_Hud::tick_lifeBar(const float& deltaTime)
{
	Player* playerRef = gameInstance().getPlayer();
	float playerHealth = playerRef->getHealth();

	// If nothing changed, skip calculations
	if (displayedHealth == playerHealth)
		return;

	int health_asInt = static_cast<int>(std::round(playerHealth * 100.0f));
	std::string health_asString = std::to_string(health_asInt);
	lifeBar.setText(health_asString);

	sf::Vector2f lifeBarSize = lifeBar.getSize();
	float newLifeBarWidth = viewSize.x * playerHealth;
	newLifeBarWidth = lerp(lifeBarSize.x, newLifeBarWidth, LERP_SMOOTHNESS);
	sf::Vector2f newLifeBarSize = sf::Vector2f(newLifeBarWidth, lifeBarSize.y);
	lifeBar.setSize(newLifeBarSize);
}

InputWidget* W_Hud::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0:
		shapes = { &lifeBar_bg, &lifeBar };
		break;
	default:
		shapes = { getActiveChild() };
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
		return this;
		break;
	}

	return nullptr;
}