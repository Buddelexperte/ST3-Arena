#pragma once

#include "W_Hud.h" // Own header file
#include "GameInstance.h"


W_Hud::W_Hud()
	: InputWidget(nullptr)
{
	const std::vector<RawButton> HUD_CONSTR = {
		{viewTL + sf::Vector2f{ 0.0f, 0.0f },	sf::Vector2f{viewSize.x, 60.0f},	sf::Color(255, 255, 255, 255),	50, "100",	sf::Color::Black,		EAlignment::LEFT_TOP, EAlignment::LEFT},
		{viewTL + sf::Vector2f{ 0.0f, 0.0f },	sf::Vector2f{viewSize.x, 60.0f},	sf::Color(255, 100, 100, 100),	0,	"",		sf::Color::Transparent, EAlignment::LEFT_TOP, EAlignment::LEFT}
	};

	lifeBar.construct(HUD_CONSTR[0]);
	lifeBar_bg.construct(HUD_CONSTR[1]);
}

void W_Hud::construct()
{
	setWidgetIndex(0);
}

void W_Hud::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
	lifeBar.setPos(	  viewTL + sf::Vector2(0.0f, 0.0f));
	lifeBar_bg.setPos(viewTL + sf::Vector2(0.0f, 0.0f));

	updateLifeBar();
}

void W_Hud::resetLifeBar()
{
	// Reset life bar to default values

	lifeBar.setText("100");

	const sf::Vector2f DEF_LIFE_BAR_SIZE = sf::Vector2f(viewSize.x, lifeBar.getSize().y);
	lifeBar.setSize(DEF_LIFE_BAR_SIZE);
}

void W_Hud::updateLifeBar()
{
	// Update the life bar based on the player's health

	// Get players health
	Player* playerRef = gameInstance().getPlayer();
	float playerHealth = playerRef->getHealth();

	// Update string displayed only if displayed health is different from actual health
	if (playerHealth != displayedHealth)
	{
		int health_asInt = static_cast<int>(std::round(playerHealth * 100.0f));
		std::string health_asString = std::to_string(health_asInt);
		lifeBar.setText(health_asString);
	}

	// Get needed width for life bar

	sf::Vector2f lifeBarSize = lifeBar.getSize();
	float newLifeBarWidth = viewSize.x * playerHealth;

	// Update life bar size if current width differs from desired width
	if (lifeBarSize.x != newLifeBarWidth)
	{
		newLifeBarWidth = lerp(lifeBarSize.x, newLifeBarWidth, LERP_SMOOTHNESS);
		sf::Vector2f newLifeBarSize = sf::Vector2f(newLifeBarWidth, lifeBarSize.y);
		lifeBar.setSize(newLifeBarSize);
	}
}

InputWidget* W_Hud::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0:
		shapes = { &lifeBar_bg, &lifeBar };
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
		return this;
		break;
	}

	return nullptr;
}