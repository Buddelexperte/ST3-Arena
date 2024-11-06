#pragma once
#include "Widgets.h"


// W_MainMenu -------------------------------------------------------------------------------------

W_MainMenu::W_MainMenu() : InputWidget()
{
	gameInstance.setGameState(MENU_SCREEN);
	
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{windowCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "ARENA",											sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, -200 },    sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16, "Higscore: " + std::to_string(SaveGame::Stored_Save),	sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "START",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "OPTIONS",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",														sf::Color::Black}
	};

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

bool W_MainMenu::isMouseOver()
{
	if (bOptionsOpen)
	{
		return optionsMenu.isMouseOver();
	}
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (menu_title->isMouseOver(mousePos))
	{
		bool alreadyChanged = (menu_title->getColor(true) == sf::Color::Red);
		menu_title->setColor((alreadyChanged ? sf::Color::White : sf::Color::Red), true);
		return true;
	}
	if (menu_startButton->isMouseOver(mousePos))
	{
		gameInstance.setGameState(GAME_LAUNCHING);
		return true;
	}
	if (menu_optionsButton->isMouseOver(mousePos))
	{
		showOptions(true);
		return true;
	}
	if (menu_quitButton->isMouseOver(mousePos))
	{
		gameInstance.setGameState(QUIT);
		return true;
	}
	// On no button-mouse overlap
	return false;
}

void W_MainMenu::showOptions(const bool& bShow)
{
	if (bOptionsOpen = bShow)
	{
		shapes = { &optionsMenu };
		return;
	}
	shapes = { menu_title, menu_highscore, menu_startButton, menu_optionsButton, menu_quitButton };
}

bool W_MainMenu::input_esc()
{
	if (bOptionsOpen) showOptions(false);
	else window->close();
	return true;
}


// W_Options --------------------------------------------------------------------------------------

W_Options::W_Options()
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

bool W_Options::isMouseOver()
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (options_return->isMouseOver(mousePos))
	{
		gameInstance.getActiveWidget()->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}

// W_Paused ---------------------------------------------------------------------------------------

W_Paused::W_Paused()
{
	const std::vector<ButtonConstruct> PAUSED_CONSTR = {
		{windowCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "PAUSE",											sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "RESUME",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "OPTIONS",													sf::Color::Black},
		{windowCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",														sf::Color::Black}
	};

	pause_title = new Button(PAUSED_CONSTR[0]);
	pause_resumeButton = new Button(PAUSED_CONSTR[1]);
	pause_optionsButton = new Button(PAUSED_CONSTR[2]);
	pause_quitButton = new Button(PAUSED_CONSTR[3]);

	shapes = { pause_title, pause_resumeButton, pause_optionsButton, pause_quitButton };
}

bool W_Paused::isMouseOver()
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (pause_resumeButton->isMouseOver(mousePos))
	{
		gameInstance.setGameState(IN_GAME);
		return true;
	}
	if (pause_quitButton->isMouseOver(mousePos))
	{
		gameInstance.setGameState(MENU_SCREEN);
		return true;
	}
	// On no button-mouse overlap
	return false;
}


// W_GameOver -------------------------------------------------------------------------------------

W_GameOver::W_GameOver(const int& currScore = 0)
{
	const std::vector<ButtonConstruct> GAME_OVER_CONSTR = {
		{windowCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "GAME OVER",							sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, -200 },    sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16, "Score: " + std::to_string(0),	sf::Color::White},
		{windowCenter + sf::Vector2f{ 0, 0 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",									sf::Color::Black}
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

bool W_GameOver::isMouseOver()
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

W_Gameplay::W_Gameplay() : InputWidget()
{
	targetController = new TargetController();
	healthBar = new Timer(10.0f, windowSize.x, 100.0f, sf::Vector2f(windowCenter.x, 0.0f));
	shapes = { targetController, &flashlightMask, healthBar };
}

void W_Gameplay::construct()
{
	if (gameInstance.getGameState() >= GAME_LAUNCHING)
	{
		unpause();
		if (gameInstance.getGameState() == GAME_LAUNCHING)
		{
			// Reset values to game start values
			hitTargets = 0;
			targetController->initSpawner(*window);
			healthBar->setMaxTime(startTimer, true);
			// Add Gameplay objects to shapes vector to draw them
			gameInstance.setGameState(IN_GAME);
		}
	}
}

void W_Gameplay::pause()
{
	if (bPaused) return;
	bPaused = true;
	shapes.push_back(&pauseScreen);
}

void W_Gameplay::unpause()
{
	if (!bPaused) return;
	bPaused = false;
	shapes.pop_back();
}

void W_Gameplay::lose()
{
	// Add GameOver Screen to shapes list
	bPaused = true;
	gameInstance.setGameState(GAME_OVER);
	shapes.push_back(&gameOverScreen);
	gameOverScreen.changeScore(hitTargets);
	if (hitTargets > SaveGame::Stored_Save) SaveGame::Stored_Save = hitTargets; // Update highscore value if new value is bigger
	SaveGame::saveData(); // Save highscore value (didn't change if no greater was achieved)
}

void W_Gameplay::update(const float& deltaTime)
{
	WidgetElement::update(deltaTime);
	// Don't update while paused
	if (bPaused) return;

	flashlightMask.update(deltaTime);
	if (gameInstance.getGameState() >= GAME_LAUNCHING)
	{
		// Update Gameplay objects with respectable params
		healthBar->update(deltaTime);
		targetController->update(*window);
		if (healthBar->isFinished()) lose();
	}
	for (sf::Drawable* elem : shapes) flashlightMask.drawOtherScene(elem);
}

bool W_Gameplay::input_esc()
{
	switch (gameInstance.getGameState())
	{
	case IN_GAME:
		pause();
		gameInstance.setGameState(GAME_PAUSED);
		break;
	case GAME_PAUSED:
		unpause();
		gameInstance.setGameState(IN_GAME);
		break;
	default:
		gameInstance.setGameState(MENU_SCREEN);
		break;
	}
	return true;
}

bool W_Gameplay::isMouseOver()
{
	if (bPaused)
	{
		switch (gameInstance.getGameState())
		{
		case GAME_PAUSED:
			return pauseScreen.isMouseOver();
			break;
		case GAME_OVER:
			return gameOverScreen.isMouseOver();
			break;
		default:
			return false;
			break;
		}
	}
	flashlightMask.resetRadius();
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (targetController->clickedAny(mousePos))
	{
		// Increase targetsHit and change HealthBar accoridngly
		hitTargets++;
		float newMaxTime = startTimer - (float(int(hitTargets) / 3) * 0.2f);
		healthBar->setMaxTime(std::max(newMaxTime, minTimer)); // Shorten HealthBar lifespan
		healthBar->setCurrentTime(healthBar->getCurrentTime() + (healthBar->getMaxTime() / 5.0f)); // Regen so a fifth of the max lifespan
		return true;
	}
	return false;
}

void W_Gameplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto elem : shapes) target.draw(*elem, states);

	return;
}