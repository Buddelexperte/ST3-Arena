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

ScoreBar::ScoreBar(InputWidget* parent)
	: WidgetElement(parent),
	scoreBar(parent), scoreBar_bg(parent), levelDisplay(parent)
{
	const std::vector<RawButton> HUD_CONSTR = {
		// Score Bar
		{(sf::Vector2f{ viewSize.x / 2.0f, viewSize.y }),								sf::Vector2f{0.0f, 5.0f * viewSizeNorm.y},					sf::Color(255, 255, 255, 255),	0,	"",		sf::Color::Transparent,		EAlignment::CENTER_BOTTOM,	EAlignment::LEFT},
		{(sf::Vector2f{ viewSize.x / 2.0f, viewSize.y }),								sf::Vector2f{viewSize.x / 3.0f, 5.0f * viewSizeNorm.y},		sf::Color(255, 255, 255, 100),	0,	"",		sf::Color::Transparent,		EAlignment::CENTER_BOTTOM,	EAlignment::RIGHT},
		// Level Display
		{(sf::Vector2f{ viewSize.x / 2.0f, viewSize.y - (20.0f * viewSizeNorm.y) }),	sf::Vector2f{50.0f, 50.0f} *viewSizeNorm,					sf::Color::Transparent,			30,	"1",	sf::Color::White,			EAlignment::CENTER_BOTTOM,	EAlignment::CENTER_BOTTOM}
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
	reset();
}

void ScoreBar::reset()
{
	// Reset score bar to default values

	const sf::Vector2f DEF_SCORE_BAR_SIZE = sf::Vector2f(0.0f, scoreBar.getSize().y);
	scoreBar.setSize(DEF_SCORE_BAR_SIZE);
}
