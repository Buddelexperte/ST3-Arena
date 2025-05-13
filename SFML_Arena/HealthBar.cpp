#pragma once

#include "HealthBar.h" // Own header file
#include "GameInstance.h"

void HealthBar::tick_bar(const float& deltaTime)
{
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

	float factor = lerpFactor(deltaTime, LERP_SMOOTHNESS);

	const sf::Vector2f lifeBarSize = lifeBar.getSize();
	if (lifeBarSize.x != newLifeBarWidth)
	{
		float actualLifeBarWidth = lerp(lifeBarSize.x, newLifeBarWidth, factor);
		sf::Vector2f newLifeBarSize = sf::Vector2f(actualLifeBarWidth, lifeBarSize.y);
		lifeBar.setSize(newLifeBarSize);
	}

	const sf::Vector2f lifeBarBgSize = lifeBar_bg.getSize();
	if (lifeBarBgSize.x != maxHealthBarWidth)
	{
		float actualLifeBarWidth = lerp(lifeBarBgSize.x, maxHealthBarWidth, factor);
		sf::Vector2f newLifeBarBgSize = sf::Vector2f(actualLifeBarWidth, lifeBarBgSize.y);
		lifeBar_bg.setSize(newLifeBarBgSize);
	}
}

HealthBar::HealthBar(InputWidget* parent)
	: WidgetElement(parent),
	lifeBar(parent), lifeBar_bg(parent)
{
	const std::vector<RawButton> HUD_CONSTR = {
		{(sf::Vector2f{20.0f, 20.0f} * unitNorm),									sf::Vector2f{0.0f, 60.0f} * unitNorm,		sf::Color(255, 255, 255, 255),	50, "100",	sf::Color::Black,			EAlignment::LEFT_TOP,		EAlignment::LEFT},
		{(sf::Vector2f{20.0f, 20.0f} * unitNorm),									sf::Vector2f{0.0f, 60.0f} * unitNorm,		sf::Color(100, 0, 0, 255),		0,	"",		sf::Color::Transparent,		EAlignment::LEFT_TOP,		EAlignment::LEFT},
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
	// No use yet
	return;
}
