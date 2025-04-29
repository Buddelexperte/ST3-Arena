#pragma once

#include "HealthBar.h" // Own header file
#include "GameInstance.h"

void HealthBar::tick_bar(const float& deltaTime)
{
	// Update the life bar based on the player's health

	// Get players health
	Player* playerRef = gameInstance().getPlayer();
	float playerHealth = playerRef->getHealth();

	// Update string displayed only if displayed health is different from actual health
	if (playerHealth != displayedHealth)
	{
		displayedHealth = playerHealth;

		int health_asInt = static_cast<int>(std::round(playerHealth * 100.0f));
		std::string health_asString = std::to_string(health_asInt);
		lifeBar.setText(health_asString);
	}

	// Get needed width for life bar
	const sf::Vector2f lifeBarSize = lifeBar.getSize();
	float newLifeBarWidth = maxHealthBarWidth * playerHealth;
	newLifeBarWidth = std::clamp(newLifeBarWidth, 0.0f, maxHealthBarWidth);

	// Update life bar size if current width differs from desired width
	if (lifeBarSize.x != newLifeBarWidth)
	{
		newLifeBarWidth = lerp(lifeBarSize.x, newLifeBarWidth, LERP_SMOOTHNESS);
		sf::Vector2f newLifeBarSize = sf::Vector2f(newLifeBarWidth, lifeBarSize.y);
		lifeBar.setSize(newLifeBarSize);
	}
}

HealthBar::HealthBar(InputWidget* parent)
	: WidgetElement(parent),
	lifeBar(parent), lifeBar_bg(parent)
{
	const std::vector<RawButton> HUD_CONSTR = {
		{(sf::Vector2f{20.0f, 20.0f} * viewSizeNorm),									sf::Vector2f{maxHealthBarWidth, 60.0f} *viewSizeNorm,		sf::Color(255, 255, 255, 255),	50, "100",	sf::Color::Black,			EAlignment::LEFT_TOP,		EAlignment::LEFT},
		{(sf::Vector2f{20.0f, 20.0f} * viewSizeNorm),									sf::Vector2f{maxHealthBarWidth, 60.0f} *viewSizeNorm,		sf::Color(100, 0, 0, 255),		0,	"",		sf::Color::Transparent,		EAlignment::LEFT_TOP,		EAlignment::LEFT},
	};

	lifeBar.construct(HUD_CONSTR[0]);
	lifeBar_bg.construct(HUD_CONSTR[1]);

	shapes = { &lifeBar_bg, &lifeBar };
}

void HealthBar::tick(const float& deltaTime)
{
	WidgetElement::tick(deltaTime);

	lifeBar.tick(deltaTime);
	lifeBar_bg.tick(deltaTime);

	tick_bar(deltaTime);
}

void HealthBar::construct()
{
	reset();
}

void HealthBar::reset()
{
	// Reset life bar to default values
	const sf::Vector2f DEFAULT_LIFE_BAR_SIZE = sf::Vector2f(maxHealthBarWidth * viewSizeNorm.x, lifeBar.getSize().y);
	lifeBar.setSize(DEFAULT_LIFE_BAR_SIZE);
}
