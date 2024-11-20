#pragma once
#include "SFML_Arena.h"
#include "Widgets.h"

// W_MainMenu -------------------------------------------------------------------------------------

W_MainMenu::W_MainMenu(InputWidget* parent) : InputWidget(parent), optionsMenu(this), levelMenu(this)
{
	// gameInstance.setGameState(MENU_SCREEN); Now handled inside teh gameInstance
	
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "ARENA",											sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, -200 },    sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16, "Higscore: " + std::to_string(SaveGame::Stored_Save),	sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "START",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "OPTIONS",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",														sf::Color::Black}
	};

	menu_title.construct(MAIN_MENU_CONSTR[0]);
	menu_highscore.construct(MAIN_MENU_CONSTR[1]);
	menu_startButton.construct(MAIN_MENU_CONSTR[2]);
	menu_optionsButton.construct(MAIN_MENU_CONSTR[3]);
	menu_quitButton.construct(MAIN_MENU_CONSTR[4]);
}

void W_MainMenu::construct()
{
	setWidgetIndex(0);
	menu_highscore.setText("Highscore: " + std::to_string(SaveGame::Stored_Save));
}

InputWidget* W_MainMenu::getWidgetAtIndex(const int& index)
{
	switch (index)
	{
	case 0:
		return this; // Self
		break;
	case 1:
		return &optionsMenu;
		break;
	case 2:
		return &levelMenu;
		break;
	default:
		break;
	}
	return nullptr;
}

InputWidget* W_MainMenu::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0: // MAIN_MENU
		shapes = { &menu_title, &menu_highscore, &menu_startButton, &menu_optionsButton, &menu_quitButton };
		break;
	case 1: // OPTIONS_MENU
		shapes = { &optionsMenu };
		break;
	case 2: // LEVEL_MENU
		shapes = { &levelMenu };
		break;
	default:
		shapes = {};
		break;
	}
	return getWidgetAtIndex(widgetIndex);
}

void W_MainMenu::update(const float& deltaTime)
{
	InputWidget::update(deltaTime);
	if (getWidgetAtIndex(widgetIndex) != this) getWidgetAtIndex(widgetIndex)->update(deltaTime);
}

void W_MainMenu::windowUpdate()
{
	InputWidget::windowUpdate();
	menu_title.setPos(viewCenter + sf::Vector2f{ 0, -300 });
	menu_highscore.setPos(viewCenter + sf::Vector2f{ 0, -200 });
	menu_startButton.setPos(viewCenter + sf::Vector2f{ 0, 0 });
	menu_optionsButton.setPos(viewCenter + sf::Vector2f{ 0, 150 });
	menu_quitButton.setPos(viewCenter + sf::Vector2f{ 0, 300 });
}

bool W_MainMenu::isMouseOver(const bool& checkForClick = false)
{
	if (getWidgetAtIndex(widgetIndex) != this)
	return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	sf::Vector2f mousePos = gameInstance.getMousePos();

	if (menu_title.isMouseOver())
	{
		sf::Color newColor = (menu_title.getColor(true) == sf::Color::White ? sf::Color::Red : sf::Color::White);
		if (checkForClick) menu_title.setColor(newColor, true);
		return true;
	}
	if (menu_startButton.isMouseOver())
	{
		if (checkForClick) setWidgetIndex(2)->construct();
		//if (checkForClick) gameInstance.setGameState(GAME_LAUNCHING); Disabled for LevelMenu testing
		return true;
	}
	if (menu_optionsButton.isMouseOver())
	{
		if (checkForClick) setWidgetIndex(true);
		return true;
	}
	if (menu_quitButton.isMouseOver())
	{
		if (checkForClick) gameInstance.setGameState(QUIT);
		return true;
	}
	// On no button-mouse overlap
	return false;
}

bool W_MainMenu::input_esc()
{
	if (widgetIndex > 0) getWidgetAtIndex(widgetIndex)->input_esc();
	else gameInstance.setGameState(QUIT);
	return true;
}


// W_Options --------------------------------------------------------------------------------------

W_Options::W_Options(InputWidget* parent = nullptr) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "OPTIONS",											sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "TEST",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "RETURN",														sf::Color::Black}
	};

	options_title.construct(MAIN_MENU_CONSTR[0]);
	options_test.construct(MAIN_MENU_CONSTR[1]);
	options_return.construct(MAIN_MENU_CONSTR[2]);

	shapes = { &options_title, &options_test, &options_return };
}

void W_Options::construct()
{
	InputWidget::construct();
}

void W_Options::windowUpdate()
{
	InputWidget::windowUpdate();
	options_title.setPos(viewCenter + sf::Vector2f{ 0, -300 });
	options_test.setPos(viewCenter + sf::Vector2f{ 0, 150 });
	options_return.setPos(viewCenter + sf::Vector2f{ 0, 300 });
}

bool W_Options::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = gameInstance.getMousePos();
	if (options_return.isMouseOver())
	{
		if (checkForClick) parent->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}

// W_LevelMenu ---------------------------------------------------------------------------------------

W_LevelMenu::W_LevelMenu(InputWidget* parent) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> LEVEL_MENU_CONSTR = {
		{viewCenter + sf::Vector2f(0.0f, -300.0f), sf::Vector2f(100.0f, 100.0f), sf::Color::Transparent, 100, "LEVEL SELECT", sf::Color::White},
		{viewCenter + sf::Vector2f(500.0f, 0.0f), sf::Vector2f(200.0f, 200.0f), sf::Color::Transparent, 24, "LEVEL 3", sf::Color::White},
		{viewCenter + sf::Vector2f(0.0f, 0.0f), sf::Vector2f(200.0f, 200.0f), sf::Color::Transparent, 24, "LEVEL 2", sf::Color::White},
		{viewCenter + sf::Vector2f(-500.0f, 0.0f), sf::Vector2f(200, 200.0f), sf::Color::Transparent, 24, "LEVEL 1", sf::Color::White},
		{viewCenter + sf::Vector2f(0.0f, 300.0f), sf::Vector2f(200.0f, 100.0f), sf::Color::White, 24, "RETURN", sf::Color::Black}
	};

	level1_Button.construct(LEVEL_MENU_CONSTR[0]);
	level2_Button.construct(LEVEL_MENU_CONSTR[1]);
	level3_Button.construct(LEVEL_MENU_CONSTR[2]);
	levelmenu_title.construct(LEVEL_MENU_CONSTR[3]);
	return_Button.construct(LEVEL_MENU_CONSTR[4]);
	
	shapes = { &level1_Button,	&level2_Button,	&level3_Button,	&levelmenu_title, &return_Button };
}

void W_LevelMenu::construct()
{
	InputWidget::construct();
}

void W_LevelMenu::update(const float& deltaTime)
{
	return;
}

bool W_LevelMenu::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = gameInstance.getMousePos();
	if (return_Button.isMouseOver())
	{
		if (checkForClick) parent->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}

// W_Paused ---------------------------------------------------------------------------------------

W_Paused::W_Paused(InputWidget* parent) : InputWidget(parent), optionsMenu(this)
{
	const std::vector<ButtonConstruct> PAUSED_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "PAUSE",											sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "RESUME",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "OPTIONS",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",														sf::Color::Black}
	};

	pause_title.construct(PAUSED_CONSTR[0]);
	pause_resumeButton.construct(PAUSED_CONSTR[1]);
	pause_optionsButton.construct(PAUSED_CONSTR[2]);
	pause_quitButton.construct(PAUSED_CONSTR[3]);
}

void W_Paused::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);
}

InputWidget* W_Paused::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0:
		return this;
		break;
	case 1:
		return &optionsMenu;
		break;
	default:
		break;
	}
	return nullptr;
}

InputWidget* W_Paused::setWidgetIndex(const int& toIndex)
{
	switch (widgetIndex = toIndex)
	{
	case 0:
		shapes = { &pause_title, &pause_resumeButton, &pause_optionsButton, &pause_quitButton };
		break;
	case 1:
		shapes = { &optionsMenu };
		break;
	default:
		shapes = {};
		break;
	}
	return getWidgetAtIndex(widgetIndex);
}

void W_Paused::update(const float& deltaTime)
{
	InputWidget::update(deltaTime);
	if (getWidgetAtIndex(widgetIndex) != this) return getWidgetAtIndex(widgetIndex)->update(deltaTime);
}

void W_Paused::windowUpdate()
{
	InputWidget::windowUpdate();
	pause_title.setPos(viewCenter + sf::Vector2f{ 0, -300 });
	pause_resumeButton.setPos(viewCenter + sf::Vector2f{ 0, 0 });
	pause_optionsButton.setPos(viewCenter + sf::Vector2f{ 0, 150 });
	pause_quitButton.setPos(viewCenter + sf::Vector2f{ 0, 300 });
}

bool W_Paused::isMouseOver(const bool& checkForClick = false)
{
	if (getWidgetAtIndex(widgetIndex) != this) return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (pause_resumeButton.isMouseOver())
	{
		if (checkForClick) gameInstance.setGameState(IN_GAME);
		return true;
	}
	if (pause_optionsButton.isMouseOver())
	{
		if (checkForClick) setWidgetIndex(1);
		return true;
	}
	if (pause_quitButton.isMouseOver())
	{
		if (checkForClick) gameInstance.setGameState(MENU_SCREEN);
		return true;
	}
	// On no button-mouse overlap
	return false;
}


// W_GameOver -------------------------------------------------------------------------------------

W_GameOver::W_GameOver(InputWidget* parent) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> GAME_OVER_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },	sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "GAME OVER",							sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, -200 },	sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16, "Score: " + std::to_string(0),	sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 0 },		sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "QUIT",									sf::Color::Black}
	};

	gameOver_title.construct(GAME_OVER_CONSTR[0]);
	gameOver_score.construct(GAME_OVER_CONSTR[1]);
	gameOver_quitButton.construct(GAME_OVER_CONSTR[2]);

	shapes = { &gameOver_title, &gameOver_score, &gameOver_quitButton };
}

void W_GameOver::windowUpdate()
{
	InputWidget::windowUpdate();

	gameOver_title.setPos(viewCenter + sf::Vector2f{ 0, -300 });
	gameOver_score.setPos(viewCenter + sf::Vector2f{ 0, -200 });
	gameOver_quitButton.setPos(viewCenter + sf::Vector2f{ 0, 0 });
}

void W_GameOver::changeScore(const int& currScore = 0)
{
	gameOver_score.setText("Score: " + std::to_string(currScore));
}

bool W_GameOver::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (gameOver_quitButton.isMouseOver())
	{
		gameInstance.setGameState(MENU_SCREEN);
		return true;
	}
	// On no button-mouse overlap
	return false;
}


// W_Gameplay -------------------------------------------------------------------------------------

W_Gameplay::W_Gameplay(InputWidget* parent) : InputWidget(parent), flashlightShader(this), pauseScreen(this), gameOverScreen(this), healthBar(10.0f, static_cast<float>(windowSize.x), 100.0f)
{
	ButtonConstruct constr = { windowCenter + sf::Vector2f(-1200.0f, 800.0f), sf::Vector2f(500.0f, 500.0f), sf::Color::White, 12, "TEST", sf::Color::Black };
	TestBox.construct(constr);
	shapes = { &targetController, &TestBox, &flashlightShader, player, &healthBar };

}

void W_Gameplay::construct()
{
	InputWidget::windowUpdate();
	if (gameInstance.getGameState() >= GAME_LAUNCHING)
	{
		if (gameInstance.getGameState() == GAME_LAUNCHING)
		{
			gameInstance.getPlayer()->setPos((sf::Vector2f(0.0f, 0.0f) + windowCenter));
			gameInstance.setViewCenter(gameInstance.getPlayer()->getPos());
			// Reset values to game start values
			hitTargets = 0;
			targetController.initSpawner();
			healthBar.setMaxTime(TIMER_DEFAULT, true);
			// Add Gameplay objects to shapes vector to draw them
			gameInstance.setGameState(IN_GAME);
		}
		unpause();
	}
}

void W_Gameplay::windowUpdate()
{
	InputWidget::windowUpdate();
}

void W_Gameplay::pause()
{
	gameInstance.setIsPaused(true);
	shapes.push_back(&pauseScreen);
	gameInstance.setGameState(GAME_PAUSED);
}

void W_Gameplay::unpause()
{
	if (pauseScreen.getWidgetIndex() > 0)
	{
		pauseScreen.construct();
		return;
	}
	gameInstance.setIsPaused(false);
	shapes = { &targetController, &TestBox, &flashlightShader, player, &healthBar };
	gameInstance.setGameState(IN_GAME);
}

void W_Gameplay::lose()
{
	// Add GameOver Screen to shapes list
	gameInstance.setIsPaused(true);
	gameInstance.setGameState(GAME_OVER);
	shapes.push_back(&gameOverScreen);
	gameOverScreen.changeScore(hitTargets);
	if (hitTargets > SaveGame::Stored_Save) SaveGame::Stored_Save = hitTargets; // Update highscore value if new value is bigger
	SaveGame::saveData(); // Save highscore value (didn't change if no greater was achieved)
}

void W_Gameplay::update(const float& deltaTime)
{
	static int frame = 0;
	frame++;
	const bool drawFlashlight = true;

	// Flashlight update
	if (drawFlashlight)
	{
		flashlightShader.update(deltaTime);
		for (sf::Drawable* elem : shapes)
		{
			flashlightShader.drawOtherScene(elem);
		}
	}

	switch (gameInstance.getGameState())
	{
	case GAME_PAUSED:
		pauseScreen.update(deltaTime);
		break;
	case GAME_OVER:
		gameOverScreen.update(deltaTime);
		break;
	default:
		InputWidget::update(deltaTime);
		break;
	}
	
	// Gameplay updates
	if (gameInstance.getGameState() >= GAME_PAUSED)
	{
		// Update Gameplay objects with respectable params
		healthBar.update(deltaTime);
		if (gameInstance.getGameState() >= GAME_LAUNCHING)
		{
			targetController.windowUpdate();
			if (healthBar.isFinished()) lose();
		}
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

bool W_Gameplay::onMouseClickR()
{
	flashlightShader.toggleMaskMode();
	return true;
}

bool W_Gameplay::isMouseOver(const bool& checkForClick = false)
{
	if (gameInstance.getIsPaused())
	{
		switch (gameInstance.getGameState())
		{
		case GAME_PAUSED:
			return pauseScreen.isMouseOver(checkForClick);
			break;
		case GAME_OVER:
			return gameOverScreen.isMouseOver(checkForClick);
			break;
		default:
			break;
		}
		return false;
	}
	if (checkForClick)
	{
		if (targetController.clickedAny(gameInstance.getMousePos()))
		{
			// Increase targetsHit and change HealthBar accoridngly
			hitTargets++;
			float newMaxTime = TIMER_DEFAULT - (float(hitTargets / 3.0f) * 0.5f); // Shorten HealthBar lifespan
			healthBar.setMaxTime(std::max(newMaxTime, minTimer)); // Keep it above minimum timer lifetime
			healthBar.setCurrentTime(healthBar.getCurrentTime() + (healthBar.getMaxTime() / 5.0f)); // Regen so a fifth of the max lifespan
			return true;
		}
		return false;
	}
	return (targetController.isHovering(gameInstance.getMousePos()));
}