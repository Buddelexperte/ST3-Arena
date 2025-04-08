#pragma once

#include "W_GameOver.h" // Own header file
#include "GameInstance.h"

W_GameOver::W_GameOver(InputWidget* parent) : InputWidget(parent)
{
	const std::vector<RawButton> GAME_OVER_CONSTR = {
		{viewCenter + sf::Vector2f{ 0.0f, -300.0f },	sf::Vector2f{ 350.0f, 120.0f }, sf::Color::Transparent,   100, "GAME OVER",						sf::Color::White},
		{viewCenter + sf::Vector2f{ 0.0f, -200.0f },	sf::Vector2f{ 100.0f, 100.0f }, sf::Color::Transparent,   16, "Score: " + std::to_string(0),	sf::Color::White},
		{viewCenter + sf::Vector2f{ 0.0f, -180.0f },	sf::Vector2f{ 100.0f, 100.0f }, sf::Color::Transparent,   16, "Kills: " + std::to_string(0),	sf::Color::White},
		{viewCenter + sf::Vector2f{ 0.0f, 0.0f },		sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "QUIT",							sf::Color::Black}
	};

	gameOver_title.construct(GAME_OVER_CONSTR[0]);
	gameOver_score.construct(GAME_OVER_CONSTR[1]);
	gameOver_kills.construct(GAME_OVER_CONSTR[2]);
	gameOver_quitButton.construct(GAME_OVER_CONSTR[3]);

	shapes = { &gameOver_title, &gameOver_score, &gameOver_kills, &gameOver_quitButton };
}

void W_GameOver::construct()
{
	InputWidget::construct();

	updateStats(SaveGame::currentData);
	// Only save if higher than stored highscore
	if (SaveGame::currentData.score > SaveGame::storedData.score)
	{
		SaveGame::saveData();
	}
}

void W_GameOver::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	gameOver_title.setPos(widgetOffset + sf::Vector2f{ 0.0f, -300.0f });
	gameOver_score.setPos(widgetOffset + sf::Vector2f{ 0.0f, -200.0f });
	gameOver_kills.setPos(widgetOffset + sf::Vector2f{ 0.0f, -180.0f });
	gameOver_quitButton.setPos(widgetOffset + sf::Vector2f{ 0.0f, 0.0f });
}

void W_GameOver::updateStats(const SaveGame_Struct& currData)
{
	gameOver_score.setText("Score: " + std::to_string(currData.score));
	gameOver_kills.setText("Kills: " + std::to_string(currData.enemiesKilled));
}

bool W_GameOver::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (gameOver_quitButton.isMouseOver(checkForClick))
	{
		gameInstance().setGameState(MENU_SCREEN);
		return true;
	}
	// On no button-mouse overlap
	return false;
}