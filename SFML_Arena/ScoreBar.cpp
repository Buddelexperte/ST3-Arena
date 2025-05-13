#pragma once

#include "ScoreBar.h" // Own header file

#include "GameInstance.h"

void ScoreBar::tick_bar(const float& deltaTime)
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

		std::string levelAsString = toRoman(playerLevel);
		levelDisplay.setText(levelAsString);
	}

	// Get needed width for score bar
	const sf::Vector2f scoreBarSize = scoreBar.getSize();
	float pointPercentageReached = static_cast<float>(playerScore - pointsLastNeeded) / static_cast<float>(playerScoreNeeded);
	pointPercentageReached = std::clamp(pointPercentageReached, 0.0f, 1.0f); // Clamp to [0, 1]
	float newScoreBarWidth = maxScoreBarWidth * pointPercentageReached;

	if (scoreBarSize.x != newScoreBarWidth)
	{
		float factor = lerpFactor(deltaTime, LERP_SMOOTHNESS);
		newScoreBarWidth = lerp(scoreBarSize.x, newScoreBarWidth, factor);
		sf::Vector2f newScoreBarSize = sf::Vector2f(newScoreBarWidth, scoreBarSize.y);
		scoreBar.setSize(newScoreBarSize);
	}

	const sf::Vector2f scoreBarBgSize = scoreBar_bg.getSize();
	if (scoreBarBgSize.x != maxScoreBarWidth)
	{
		float factor = lerpFactor(deltaTime, LERP_SMOOTHNESS);
		newScoreBarWidth = lerp(scoreBarBgSize.x, maxScoreBarWidth, factor);
		sf::Vector2f newScoreBarBgSize = sf::Vector2f(newScoreBarWidth, scoreBarBgSize.y);
		scoreBar_bg.setSize(newScoreBarBgSize);
	}
}

ScoreBar::ScoreBar(InputWidget* parent)
	: WidgetElement(parent),
	scoreBar(parent), scoreBar_bg(parent), levelDisplay(parent)
{
	const std::vector<RawButton> HUD_CONSTR = {
		// Score Bar
		{(sf::Vector2f{ viewSize.x / 2.0f, viewSize.y }),								sf::Vector2f{0.0f, 5.0f * unitNorm.y},	sf::Color(255, 255, 255, 255),	0,	"",		sf::Color::Transparent,		EAlignment::CENTER_BOTTOM,	EAlignment::LEFT},
		{(sf::Vector2f{ viewSize.x / 2.0f, viewSize.y }),								sf::Vector2f{0.0f, 5.0f * unitNorm.y},	sf::Color(255, 255, 255, 100),	0,	"",		sf::Color::Transparent,		EAlignment::CENTER_BOTTOM,	EAlignment::RIGHT},
		// Level Display
		{(sf::Vector2f{ viewSize.x / 2.0f, viewSize.y - (20.0f * unitNorm.y) }),	sf::Vector2f{50.0f, 50.0f} * unitNorm,	sf::Color::Transparent,			30,	"",		sf::Color::White,			EAlignment::CENTER_BOTTOM,	EAlignment::CENTER_BOTTOM}
	};

	scoreBar.construct(HUD_CONSTR[0]);
	scoreBar_bg.construct(HUD_CONSTR[1]);
	levelDisplay.construct(HUD_CONSTR[2]);

	shapes = { &scoreBar_bg, &scoreBar, &levelDisplay };
}

void ScoreBar::tick(const float& deltaTime)
{
	WidgetElement::tick(deltaTime);

	scoreBar.tick(deltaTime);
	scoreBar_bg.tick(deltaTime);
	levelDisplay.tick(deltaTime);

	tick_bar(deltaTime);
}

void ScoreBar::construct()
{
	scoreBar.setSize(sf::Vector2f(0.0f, scoreBar_bg.getSize().y));
	scoreBar_bg.setSize(sf::Vector2f(0.0f, scoreBar_bg.getSize().y));
}

void ScoreBar::reset()
{
	// Reset score bar to default values

	const sf::Vector2f DEF_SCORE_BAR_SIZE = sf::Vector2f(0.0f, scoreBar.getSize().y);
	scoreBar.setSize(DEF_SCORE_BAR_SIZE);
}
