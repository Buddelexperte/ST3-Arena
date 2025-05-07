#pragma once

#include "W_TitleScreen.h" // Own header file
#include "GameInstance.h"
#include "SaveGame.h"



W_TitleScreen::W_TitleScreen(InputWidget* parent)
	: InputWidget(parent),
	T_Title(this), T_Highscore(this), B_Start(this), B_OptionsMenu(this), B_Quit(this)
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		{(sf::Vector2f{ 0.0f, -300.0f }	* unitNorm),    sf::Vector2f{ 350, 120 } * unitNorm,	sf::Color::Transparent,		100,"ARENA",							sf::Color::White},
		{(sf::Vector2f{ 0.0f, -150.0f }	* unitNorm),    sf::Vector2f{ 100, 100 } * unitNorm,	sf::Color::Transparent,		16,	"Higscore: " + std::to_string(0),	sf::Color::White},
		{(sf::Vector2f{ 0.0f, 0.0f }	* unitNorm),    buttonSize,								sf::Color::White,			24,	"START",							sf::Color::Black},
		{(sf::Vector2f{ 0.0f, 150.0f }	* unitNorm),    buttonSize,								sf::Color::White,			24,	"OPTIONS",							sf::Color::Black},
		{(sf::Vector2f{ 0.0f, 300.0f }	* unitNorm),    buttonSize,								sf::Color::White,			24,	"QUIT",								sf::Color::Black}
	};

	T_Title.construct(MAIN_MENU_CONSTR[0]);
	T_Highscore.construct(MAIN_MENU_CONSTR[1]);
	B_Start.construct(MAIN_MENU_CONSTR[2]);
	B_OptionsMenu.construct(MAIN_MENU_CONSTR[3]);
	B_Quit.construct(MAIN_MENU_CONSTR[4]);

	delegateButtons();

	shapes = { &T_Title, &T_Highscore, &B_Start, &B_OptionsMenu, &B_Quit };
}

void W_TitleScreen::delegateButtons()
{
	T_Title.onClick = [this]()
	{
		bool bIsWhite = (T_Title.getColor(true) == sf::Color::White);
		sf::Color newColor = (bIsWhite ? sf::Color::Red : sf::Color::White);
		T_Title.setColor(newColor, true);
	};

	B_Start.onClick = [this]()
	{
		parent->setWidgetIndex(2)->construct();
	};

	B_OptionsMenu.onClick = [this]()
	{
		parent->setWidgetIndex(1)->construct();
	};

	B_Quit.onClick = [this]()
	{
		gameInstance().setGameState(QUIT);
	};
}

void W_TitleScreen::construct()
{
	InputWidget::construct();

	gameInstance().resetWindowName();
	T_Highscore.setText("Highscore: " + std::to_string(SaveGame::storedData.score));
}

void W_TitleScreen::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);
	T_Highscore.tick(deltaTime);
	B_Start.tick(deltaTime);
	B_OptionsMenu.tick(deltaTime);
	B_Quit.tick(deltaTime);
}

bool W_TitleScreen::isMouseOver(const bool& checkForClick = false)
{
	if (T_Title.isMouseOver(checkForClick))
		return true;

	if (B_Start.isMouseOver(checkForClick))
		return true;

	if (B_OptionsMenu.isMouseOver(checkForClick))
		return true;

	if (B_Quit.isMouseOver(checkForClick))
		return true;

	return false;
}

bool W_TitleScreen::onKeyEscape()
{
	gameInstance().setGameState(QUIT);
	return true;
}
