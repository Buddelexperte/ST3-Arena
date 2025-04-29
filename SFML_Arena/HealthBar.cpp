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
	float newLifeBarWidth = maxHealthBarWidth * playerHealth;
	newLifeBarWidth = std::clamp(newLifeBarWidth, 0.0f, maxHealthBarWidth);

	const sf::Vector2f lifeBarSize = lifeBar.getSize();
	if (lifeBarSize.x != newLifeBarWidth)
	{
		newLifeBarWidth = lerp(lifeBarSize.x, newLifeBarWidth, LERP_SMOOTHNESS);
		sf::Vector2f newLifeBarSize = sf::Vector2f(newLifeBarWidth, lifeBarSize.y);
		lifeBar.setSize(newLifeBarSize);
	}

	const sf::Vector2f lifeBarBgSize = lifeBar_bg.getSize();
	if (lifeBarBgSize.x != maxHealthBarWidth)
	{
		newLifeBarWidth = lerp(lifeBarBgSize.x, maxHealthBarWidth, LERP_SMOOTHNESS);
		sf::Vector2f newLifeBarBgSize = sf::Vector2f(newLifeBarWidth, lifeBarBgSize.y);
		lifeBar_bg.setSize(newLifeBarBgSize);
	}
}

HealthBar::HealthBar(InputWidget* parent)
	: WidgetElement(parent),
	lifeBar(parent), lifeBar_bg(parent)
{
	const std::vector<RawButton> HUD_CONSTR = {
		{(sf::Vector2f{20.0f, 20.0f} * viewSizeNorm),									sf::Vector2f{0.0f, 60.0f} * viewSizeNorm,		sf::Color(255, 255, 255, 255),	50, "100",	sf::Color::Black,			EAlignment::LEFT_TOP,		EAlignment::LEFT},
		{(sf::Vector2f{20.0f, 20.0f} * viewSizeNorm),									sf::Vector2f{0.0f, 60.0f} * viewSizeNorm,		sf::Color(100, 0, 0, 255),		0,	"",		sf::Color::Transparent,		EAlignment::LEFT_TOP,		EAlignment::LEFT},
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
	lifeBar.setSize(sf::Vector2f(0.0f, lifeBar_bg.getSize().y));
	lifeBar_bg.setSize(sf::Vector2f(0.0f, lifeBar_bg.getSize().y));
}

void HealthBar::reset()
{
	// Reset life bar to default values

	if (displayedHealth == 1.0f)
		return;

	const sf::Vector2f DEFAULT_LIFE_BAR_SIZE = sf::Vector2f(0.0f, lifeBar.getSize().y);
	lifeBar.setSize(DEFAULT_LIFE_BAR_SIZE);
}
