#pragma once

#include "W_GameOver.h" // Own header file
#include "GameInstance.h"

W_GameOver::W_GameOver(InputWidget* parent) 
	: InputWidget(parent),
	gameOver_title(this), gameOver_score(this), gameOver_kills(this), gameOver_quitButton(this)
{
	const std::vector<RawButton> GAME_OVER_CONSTR = {
		{sf::Vector2f{ 0.0f, -300.0f },		sf::Vector2f{ 350.0f, 120.0f },		sf::Color::Transparent, 100,	"GAME OVER",					sf::Color::White},
		{sf::Vector2f{ 0.0f, -200.0f },		sf::Vector2f{ 100.0f, 100.0f },		sf::Color::Transparent, 16,		"Score: " + std::to_string(0),	sf::Color::White},
		{sf::Vector2f{ 0.0f, -180.0f },		sf::Vector2f{ 100.0f, 100.0f },		sf::Color::Transparent, 16,		"Kills: " + std::to_string(0),	sf::Color::White},
		{sf::Vector2f{ 0.0f, 0.0f },		buttonSize,							sf::Color::White,	24,			"QUIT",							sf::Color::Black}
	};

	gameOver_title.construct(GAME_OVER_CONSTR[0]);
	gameOver_score.construct(GAME_OVER_CONSTR[1]);
	gameOver_kills.construct(GAME_OVER_CONSTR[2]);
	gameOver_quitButton.construct(GAME_OVER_CONSTR[3]);

	shapes = { &gameOver_title, &gameOver_score, &gameOver_kills, &gameOver_quitButton };
}

sf::Vector2f W_GameOver::getCorrectTickCorrection() const
{
	return widgetOffset;
}

void W_GameOver::construct()
{
	InputWidget::construct();

	gameInstance().modWindowName(" - You lost!");
	gameInstance().setGameState(GAME_OVER);

	updateStats(SaveGame::currentData);
}

void W_GameOver::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	gameOver_title.tick(deltaTime);
	gameOver_score.tick(deltaTime);
	gameOver_kills.tick(deltaTime);
	gameOver_quitButton.tick(deltaTime);
}

void W_GameOver::updateStats(const SaveGame_Struct& currData)
{
	gameOver_score.setText("Score: " + std::to_string(currData.score));
	gameOver_kills.setText("Kills: " + std::to_string(currData.enemiesKilled));
}

bool W_GameOver::isMouseOver(const bool& checkForClick = false)
{
	if (gameOver_quitButton.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->playAnim(EAnimation::CLOSE_ANIM);
		return true;
	}
	// On no button-mouse overlap
	return false;
}

bool W_GameOver::onKeyEscape()
{
	return false;
}