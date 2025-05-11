#pragma once

#include "W_GameOver.h" // Own header file
#include "GameInstance.h"

W_GameOver::W_GameOver(InputWidget* parent) 
	: InputWidget(parent),
	T_Title(this), T_Score(this), T_Kills(this), B_Quit(this)
{
	const std::vector <RawText> TEXT_CONSTR = {
		{sf::Vector2f( 0.0f, -300.0f ), sf::Color::White, 100, "GAME OVER"					},
		{sf::Vector2f( 0.0f, -200.0f ), sf::Color::White, 16, "Score: " + std::to_string(0)	},
		{sf::Vector2f( 0.0f, -180.0f ), sf::Color::White, 16, "Kills: " + std::to_string(0)	}
	};

	const RawButton GAME_OVER_CONSTR = {
		sf::Vector2f{ 0.0f, 0.0f },		buttonSize,							sf::Color::White,	24,			"QUIT",							sf::Color::Black
	};

	T_Title.construct(TEXT_CONSTR[0]);
	T_Score.construct(TEXT_CONSTR[1]);
	T_Kills.construct(TEXT_CONSTR[2]);
	B_Quit.construct(GAME_OVER_CONSTR);

	delegateButtons();

	shapes = { &T_Title, &T_Score, &T_Kills, &B_Quit };
}

void W_GameOver::delegateButtons()
{
	B_Quit.onClick = [this]()
	{
		parent->playAnim(EAnimation::CLOSE_ANIM);
	};
}

sf::Vector2f W_GameOver::getTickCorrection() const
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

	T_Title.tick(deltaTime);
	T_Score.tick(deltaTime);
	T_Kills.tick(deltaTime);
	B_Quit.tick(deltaTime);
}

void W_GameOver::updateStats(const SaveGame_Struct& currData)
{
	T_Score.setText("Score: " + std::to_string(currData.score));
	T_Kills.setText("Kills: " + std::to_string(currData.enemiesKilled));
}

bool W_GameOver::isMouseOver(const bool& checkForClick = false)
{
	if (B_Quit.isMouseOver(checkForClick))
		return true;

	// On no button-mouse overlap
	return false;
}

bool W_GameOver::onKeyEscape()
{
	return false;
}