#pragma once
#include "SFML_Arena.h"


// W_MainMenu -------------------------------------------------------------------------------------

W_MainMenu::W_MainMenu(WidgetElement* parent) : InputWidget(parent)
{
	gameInstance.setGameState(MENU_SCREEN);
	
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{windowCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "ARENA",											sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, -200 },    sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16, "Higscore: " + std::to_string(SaveGame::Stored_Save),	sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "START",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "OPTIONS",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",														sf::Color::Black}
	};

	optionsMenu = new W_Options(this);

	menu_title = new Button(MAIN_MENU_CONSTR[0]);
	menu_highscore = new Button(MAIN_MENU_CONSTR[1]);
	menu_startButton = new Button(MAIN_MENU_CONSTR[2]);
	menu_optionsButton = new Button(MAIN_MENU_CONSTR[3]);
	menu_quitButton = new Button(MAIN_MENU_CONSTR[4]);

	shapes = { menu_title, menu_highscore, menu_startButton, menu_optionsButton, menu_quitButton };
}

void W_MainMenu::construct()
{
	menu_highscore->setText("Highscore: " + std::to_string(SaveGame::Stored_Save));
	showOptions(false);
}

bool W_MainMenu::isMouseOver(const bool& checkForClick = false)
{
	if (bOptionsOpen)
	{
		return optionsMenu->isMouseOver(checkForClick);
	}
	sf::Vector2f mousePos = gameInstance.getMousePos();
	if (menu_title->isMouseOver(mousePos))
	{
		sf::Color newColor = (menu_title->getColor(true) == sf::Color::White ? sf::Color::Red : sf::Color::White);
		if (checkForClick) menu_title->setColor(newColor, true);
		return true;
	}
	if (menu_startButton->isMouseOver(mousePos))
	{
		if (checkForClick) gameInstance.setGameState(GAME_LAUNCHING);
		return true;
	}
	if (menu_optionsButton->isMouseOver(mousePos))
	{
		if (checkForClick) showOptions(true);
		return true;
	}
	if (menu_quitButton->isMouseOver(mousePos))
	{
		if (checkForClick) gameInstance.setGameState(QUIT);
		return true;
	}
	// On no button-mouse overlap
	return false;
}

void W_MainMenu::showOptions(const bool& bShow)
{
	if (bOptionsOpen = bShow)
	{
		shapes = { optionsMenu };
		return;
	}
	shapes = { menu_title, menu_highscore, menu_startButton, menu_optionsButton, menu_quitButton };
}

bool W_MainMenu::input_esc()
{
	if (bOptionsOpen) showOptions(false);
	else gameInstance.setGameState(QUIT);
	return true;
}


// W_Options --------------------------------------------------------------------------------------

W_Options::W_Options(WidgetElement* parent = nullptr) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{windowCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "OPTIONS",											sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "TEST",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "RETURN",														sf::Color::Black}
	};

	options_title = new Button(MAIN_MENU_CONSTR[0]);
	options_test = new Button(MAIN_MENU_CONSTR[1]);
	options_return = new Button(MAIN_MENU_CONSTR[2]);

	shapes = { options_title, options_test, options_return };
}

bool W_Options::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = gameInstance.getMousePos();
	if (options_return->isMouseOver(mousePos))
	{
		if (checkForClick) parent->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}

bool W_Options::input_esc()
{
	if (parent != nullptr) parent->construct();
	return true;
}

// W_Paused ---------------------------------------------------------------------------------------

W_Paused::W_Paused(WidgetElement* parent) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> PAUSED_CONSTR = {
		{windowCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "PAUSE",											sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "RESUME",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "OPTIONS",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",														sf::Color::Black}
	};

	optionsMenu = new W_Options(this);

	pause_title = new Button(PAUSED_CONSTR[0]);
	pause_resumeButton = new Button(PAUSED_CONSTR[1]);
	pause_optionsButton = new Button(PAUSED_CONSTR[2]);
	pause_quitButton = new Button(PAUSED_CONSTR[3]);

	shapes = { pause_title, pause_resumeButton, pause_optionsButton, pause_quitButton };
}

void W_Paused::construct()
{
	showOptions(false);
}

bool W_Paused::input_esc()
{
	showOptions(!bOptionsOpen);
	return bOptionsOpen;
}

void W_Paused::showOptions(const bool& bShow)
{
	if (bOptionsOpen = bShow)
	{
		shapes = { optionsMenu };
		return;
	}
	optionsMenu->construct();
	shapes = { pause_title, pause_resumeButton, pause_optionsButton, pause_quitButton };
}

bool W_Paused::isMouseOver(const bool& checkForClick = false)
{
	if (bOptionsOpen) return optionsMenu->isMouseOver(checkForClick);

	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (pause_resumeButton->isMouseOver(mousePos))
	{
		if (checkForClick) gameInstance.setGameState(IN_GAME);
		return true;
	}
	if (pause_optionsButton->isMouseOver(mousePos))
	{
		if (checkForClick) showOptions(true);
		return true;
	}
	if (pause_quitButton->isMouseOver(mousePos))
	{
		if (checkForClick) gameInstance.setGameState(MENU_SCREEN);
		return true;
	}
	// On no button-mouse overlap
	return false;
}


// W_GameOver -------------------------------------------------------------------------------------

W_GameOver::W_GameOver(WidgetElement* parent) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> GAME_OVER_CONSTR = {
		{windowCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "GAME OVER",							sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, -200 },    sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16, "Score: " + std::to_string(0),	sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, 0 },		sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",									sf::Color::Black}
	};

	gameOver_title = new Button(GAME_OVER_CONSTR[0]);
	gameOver_score = new Button(GAME_OVER_CONSTR[1]);
	gameOver_quitButton = new Button(GAME_OVER_CONSTR[2]);

	shapes = { gameOver_title, gameOver_score, gameOver_quitButton };
}

void W_GameOver::changeScore(const int& currScore = 0)
{
	gameOver_score->setText("Score: " + std::to_string(currScore));
}

bool W_GameOver::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (gameOver_quitButton->isMouseOver(mousePos))
	{
		gameInstance.setGameState(MENU_SCREEN);
		return true;
	}
	// On no button-mouse overlap
	return false;
}


// W_Gameplay -------------------------------------------------------------------------------------

W_Gameplay::W_Gameplay(WidgetElement* parent) : InputWidget(parent)
{
	pauseScreen = new W_Paused(this);
	gameOverScreen = new W_GameOver(this);

	targetController = new TargetController();
	healthBar = new Timer(10.0f, windowSize.x, 100.0f, sf::Vector2f(windowCenter.x, 0.0f));
	shapes = { targetController, &flashlightMask, player, healthBar };
}

void W_Gameplay::construct()
{
	if (gameInstance.getGameState() >= GAME_LAUNCHING)
	{
		if (gameInstance.getGameState() == GAME_LAUNCHING)
		{
			// Reset values to game start values
			hitTargets = 0;
			targetController->initSpawner();
			healthBar->setMaxTime(TIMER_DEFAULT, true);
			// Add Gameplay objects to shapes vector to draw them
			gameInstance.setGameState(IN_GAME);
		}
		unpause();
	}
}

void W_Gameplay::pause()
{
	gameInstance.setIsPaused(true);
	shapes.push_back(pauseScreen);
	gameInstance.setGameState(GAME_PAUSED);
}

void W_Gameplay::unpause()
{
	if (pauseScreen->isSubMenuOpen())
	{
		pauseScreen->construct();
		return;
	}
	gameInstance.setIsPaused(false);
	shapes = { targetController, &flashlightMask, player, healthBar };
	gameInstance.setGameState(IN_GAME);
}

void W_Gameplay::lose()
{
	// Add GameOver Screen to shapes list
	gameInstance.setIsPaused(true);
	gameInstance.setGameState(GAME_OVER);
	shapes.push_back(gameOverScreen);
	gameOverScreen->changeScore(hitTargets);
	if (hitTargets > SaveGame::Stored_Save) SaveGame::Stored_Save = hitTargets; // Update highscore value if new value is bigger
	SaveGame::saveData(); // Save highscore value (didn't change if no greater was achieved)
}

void W_Gameplay::update(const float& deltaTime)
{
	InputWidget::update(deltaTime);
	// Don't update while paused
	if (gameInstance.getIsPaused()) return;

	flashlightMask.update(deltaTime);
	if (gameInstance.getGameState() >= GAME_LAUNCHING)
	{
		// Update Gameplay objects with respectable params
		healthBar->update(deltaTime);
		targetController->windowUpdate();
		if (healthBar->isFinished()) lose();
	}
	for (sf::Drawable* elem : shapes)
	{
		flashlightMask.drawOtherScene(elem);
	}
}

bool W_Gameplay::input_esc()
{
	switch (gameInstance.getGameState())
	{
	case IN_GAME:
		pause();
		break;
	case GAME_PAUSED:
		unpause();
		break;
	default:
		gameInstance.setGameState(MENU_SCREEN);
		break;
	}
	return true;
}

bool W_Gameplay::isMouseOver(const bool& checkForClick = false)
{
	if (gameInstance.getIsPaused())
	{
		switch (gameInstance.getGameState())
		{
		case GAME_PAUSED:
			return pauseScreen->isMouseOver(checkForClick);
			break;
		case GAME_OVER:
			return gameOverScreen->isMouseOver(checkForClick);
			break;
		default:
			break;
		}
		return false;
	}
	flashlightMask.resetRadius();
	if (checkForClick)
	{
		if (targetController->clickedAny(gameInstance.getMousePos()))
		{
			// Increase targetsHit and change HealthBar accoridngly
			hitTargets++;
			float newMaxTime = TIMER_DEFAULT - (float(hitTargets / 3.0f) * 0.5f); // Shorten HealthBar lifespan
			healthBar->setMaxTime(std::max(newMaxTime, minTimer)); // Keep it above minimum timer lifetime
			healthBar->setCurrentTime(healthBar->getCurrentTime() + (healthBar->getMaxTime() / 5.0f)); // Regen so a fifth of the max lifespan
			return true;
		}
		return false;
	}
	return (targetController->isHovering(gameInstance.getMousePos()));
}