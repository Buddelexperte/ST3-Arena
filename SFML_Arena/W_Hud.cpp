#pragma once

#include "W_Hud.h" // Own header file
#include "GameInstance.h"


W_Hud::W_Hud()
	: InputWidget(nullptr)
{
	const std::vector<RawButton> HUD_CONSTR = {
		// Health bar
		{viewTL + sf::Vector2f{ 0.0f, 0.0f },						sf::Vector2f{viewSize.x, 60.0f},		sf::Color(255, 255, 255, 255),	50, "100",	sf::Color::Black,		EAlignment::LEFT_TOP,		EAlignment::LEFT},
		{viewTL + sf::Vector2f{ 0.0f, 0.0f },						sf::Vector2f{viewSize.x, 60.0f},		sf::Color(255, 100, 100, 100),	0,	"",		sf::Color::Transparent, EAlignment::LEFT_TOP,		EAlignment::LEFT},
		// Score Bar
		{viewTL + sf::Vector2f{ viewSize.x / 2.0f, viewSize.y },	sf::Vector2f{0.0f, 10.0f},				sf::Color(255, 255, 255, 255),	0,	"",		sf::Color::Transparent, EAlignment::CENTER_BOTTOM,	EAlignment::LEFT},
		{viewTL + sf::Vector2f{ viewSize.x / 2.0f, viewSize.y },	sf::Vector2f{viewSize.x / 3.0f, 10.0f},	sf::Color(255, 255, 255, 100),	0,	"",		sf::Color::Transparent, EAlignment::CENTER_BOTTOM,	EAlignment::RIGHT},
		// Level Display
		{viewTL + sf::Vector2f{ viewSize.x / 2.0f, viewSize.y - 30.0f },	sf::Vector2f{50.0f, 50.0f},		sf::Color::Transparent,			30,	"1",	sf::Color::White,		EAlignment::CENTER_BOTTOM,	EAlignment::CENTER_BOTTOM}
	};

	lifeBar.construct(HUD_CONSTR[0]);
	lifeBar_bg.construct(HUD_CONSTR[1]);
	scoreBar.construct(HUD_CONSTR[2]);
	scoreBar_bg.construct(HUD_CONSTR[3]);
	levelDisplay.construct(HUD_CONSTR[4]);
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
	scoreBar.setPos(viewTL + sf::Vector2f{ viewSize.x / 2.0f, viewSize.y });
	scoreBar_bg.setPos(viewTL + sf::Vector2f{ viewSize.x / 2.0f, viewSize.y });
	levelDisplay.setPos(viewTL + sf::Vector2f{ viewSize.x / 2.0f, viewSize.y });

	updateLifeBar();
	updateScoreBar();
}

void W_Hud::resetLifeBar()
{
	// Reset life bar to default values

	lifeBar.setText("100");
	displayedHealth = 100.0f;

	const sf::Vector2f DEFAULT_LIFE_BAR_SIZE = sf::Vector2f(viewSize.x, lifeBar.getSize().y);
	lifeBar.setSize(DEFAULT_LIFE_BAR_SIZE);
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
		displayedHealth = playerHealth;

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

void W_Hud::resetScoreBar()
{
	// Reset score bar to default values

	levelDisplay.setText("1");
	displayedLevel = 1;

	const sf::Vector2f DEF_SCORE_BAR_SIZE = sf::Vector2f(0.0f, scoreBar.getSize().y);
	scoreBar.setSize(DEF_SCORE_BAR_SIZE);
}

void W_Hud::updateScoreBar()
{
	// Update the score bar based on the player's score

	// Get players health
	Player* playerRef = gameInstance().getPlayer();
	LevelSystem& ls = playerRef->getInventory().getLevelSystem();
	unsigned int playerScore = ls.getPoints();
	unsigned int pointsLastNeeded = ls.getLastPointsNeeded();
	unsigned int playerScoreNeeded = ls.getPointsNeeded();
	unsigned int playerLevel = ls.getStage();

	// Update string displayed only if displayed stage is different from actual stage
	if (playerLevel != displayedLevel)
	{
		displayedLevel = playerLevel;

		std::string levelAsString = std::to_string(playerLevel);
		levelDisplay.setText(levelAsString);
	}

	// Get needed width for score bar
	sf::Vector2f scoreBarSize = scoreBar.getSize();
	const float maxWidth = viewSize.x / 3.0f;
	float newScoreBarWidth = maxWidth * (static_cast<float>(playerScore - pointsLastNeeded) / static_cast<float>(playerScoreNeeded));

	// Update life bar size if current width differs from desired width
	if (scoreBarSize.x != newScoreBarWidth)
	{
		newScoreBarWidth = lerp(scoreBarSize.x, newScoreBarWidth, LERP_SMOOTHNESS);
		sf::Vector2f newScoreBarSize = sf::Vector2f(newScoreBarWidth, scoreBarSize.y);
		scoreBar.setSize(newScoreBarSize);
	}
}

InputWidget* W_Hud::setWidgetIndex(const int& newIndex)
{
	// Default drawables used
	shapes = { &lifeBar_bg, &lifeBar, &scoreBar, &scoreBar_bg, &levelDisplay };

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