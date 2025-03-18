#pragma once

#include "Widgets.h" // Header File
#include "SaveGame.h"

// W_MainMenu -------------------------------------------------------------------------------------

W_MainMenu::W_MainMenu(InputWidget* parent)
	: InputWidget(parent), optionsMenu(this), levelMenu(this), inventory(this)
{
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100,	"ARENA",													sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, -200 },    sf::Vector2f{ 100, 100 }, sf::Color::Transparent,   16,	"Higscore: " + std::to_string(SaveGame::storedData.score),	sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 0 },       sf::Vector2f{ 300, 100 }, sf::Color::White,         24,	"START",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 150 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24,	"OPTIONS",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 300 },     sf::Vector2f{ 300, 100 }, sf::Color::White,         24,	"QUIT",														sf::Color::Black}
	};

	menu_title.construct(MAIN_MENU_CONSTR[0]);
	menu_highscore.construct(MAIN_MENU_CONSTR[1]);
	menu_startButton.construct(MAIN_MENU_CONSTR[2]);
	menu_optionsButton.construct(MAIN_MENU_CONSTR[3]);
	menu_quitButton.construct(MAIN_MENU_CONSTR[4]);

	// Done out
	std::cout << "- Constructed MainMenu" << std::endl;
}

void W_MainMenu::construct()
{
	setWidgetIndex(0);
	menu_highscore.setText("Highscore: " + std::to_string(SaveGame::storedData.score));
}

InputWidget* W_MainMenu::getWidgetAtIndex(const int& index)
{
	switch (index)
	{
	case 0:
		return this; // Self
		break;
	case 1:
		return &optionsMenu; // OPTIONS_MENU
		break;
	case 2:
		return &levelMenu; // LEVEL_MENU
		break;
	case 3:
		return &inventory;
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
	default: // SUB-WIDGETS (implement this notation in EVERY other Widget)
		shapes = { getWidgetAtIndex(widgetIndex) };
		break;
	}
	return getWidgetAtIndex(widgetIndex);
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
	if (isChildActive())
		return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	sf::Vector2f mousePos = gameInstance->getMousePos();

	if (menu_title.isMouseOver(checkForClick))
	{
		sf::Color newColor = (menu_title.getColor(true) == sf::Color::White ? sf::Color::Red : sf::Color::White);
		if (checkForClick) menu_title.setColor(newColor, true);
		return true;
	}
	if (menu_startButton.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(2)->construct();
		return true;
	}
	if (menu_optionsButton.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(1)->construct();
		return true;
	}
	if (menu_quitButton.isMouseOver(checkForClick))
	{
		if (checkForClick) gameInstance->setGameState(QUIT);
		return true;
	}
	// On no button-mouse overlap
	return false;
}

bool W_MainMenu::input_esc()
{
	if (widgetIndex > 0) getWidgetAtIndex(widgetIndex)->input_esc();
	else gameInstance->setGameState(QUIT);
	return true;
}

// OptionSoundMenu ------------------------------------------------------------------------------------

W_OptionsSounds::W_OptionsSounds(InputWidget* parent = nullptr) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },    sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "SOUND",		sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 300 },		sf::Vector2f{ 300, 100 }, sf::Color::White,        24, "RETURN",	sf::Color::Black},
	};

	optionsSounds_test.construct(MAIN_MENU_CONSTR[0]);
}

void W_OptionsSounds::construct()
{
	InputWidget::construct();
	shapes = { &optionsSounds_test };
}

void W_OptionsSounds::windowUpdate()
{
	InputWidget::windowUpdate();
	optionsSounds_test.setPos(viewCenter + sf::Vector2f{ 0, -300 });
}

bool W_OptionsSounds::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = gameInstance->getMousePos();
	if (optionsSounds_test.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}

// OptionGraphicsMenu ------------------------------------------------------------------------------------

W_OptionsGraphics::W_OptionsGraphics(InputWidget* parent = nullptr) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 }, sf::Vector2f{ 350, 120 }, sf::Color::Transparent,		100, "Graphics", sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 300 },		sf::Vector2f{ 300, 100 }, sf::Color::White,         24,  "RETURN",   sf::Color::Black},

	};

	optionsGraphics_test.construct(MAIN_MENU_CONSTR[0]);
}

void W_OptionsGraphics::construct()
{
	InputWidget::construct();
	shapes = { &optionsGraphics_test };
}

void W_OptionsGraphics::windowUpdate()
{
	InputWidget::windowUpdate();
	optionsGraphics_test.setPos(viewCenter + sf::Vector2f{ 0, -300 });
}

bool W_OptionsGraphics::isMouseOver(const bool& checkForClick = false)
{
	if (optionsGraphics_test.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}

// W_Options --------------------------------------------------------------------------------------

W_Options::W_Options(InputWidget* parent = nullptr) : InputWidget(parent), soundMenu(this), graphicMenu(this)
{
	const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
		{viewCenter + sf::Vector2f{ 0, -300 },		sf::Vector2f{ 350, 120 }, sf::Color::Transparent,   100, "OPTIONS",											sf::Color::White},
		{viewCenter + sf::Vector2f{ 0, 0 },			sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "SOUNDS",														sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 150 },		sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "GRAPHICS",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0, 300 },		sf::Vector2f{ 300, 100 }, sf::Color::White,         24, "RETURN",														sf::Color::Black},
	};

	options_title.construct(MAIN_MENU_CONSTR[0]);
	options_graphics.construct(MAIN_MENU_CONSTR[1]);
	options_sounds.construct(MAIN_MENU_CONSTR[2]);
	options_return.construct(MAIN_MENU_CONSTR[3]);
}

InputWidget* W_Options::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0: // OPTIONS SELECTOR
		return this;
		break;
	case 1: // SOUNDS
		return &soundMenu;
		break;
	case 2: // GRAPHICS
		return &graphicMenu;
		break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
}

InputWidget* W_Options::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0: // THIS
		shapes = { &options_title, &options_graphics, &options_return, &options_sounds };
		break;
	case 1: // SOUNDS
		shapes = { &soundMenu };
		break;
	case 2: // GRAPHICS
		shapes = { &graphicMenu };
		break;
	default:
		shapes = {};
		break;
	}
	return getWidgetAtIndex(widgetIndex);
}

void W_Options::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);
}

void W_Options::windowUpdate()
{
	InputWidget::windowUpdate();
	options_title.setPos(viewCenter + sf::Vector2f{ 0, -300 });
	options_graphics.setPos(viewCenter + sf::Vector2f{ 0, 0 });
	options_sounds.setPos(viewCenter + sf::Vector2f{ 0, 150 });
	options_return.setPos(viewCenter + sf::Vector2f{ 0, 300 });
}

bool W_Options::isMouseOver(const bool& checkForClick)
{
	if (getWidgetAtIndex(widgetIndex) != this)
	return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	if (options_return.isMouseOver(checkForClick))
	{
		if (checkForClick) parent->construct();
		return true;
	}
	if (options_graphics.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(1)->construct();
		return true;
	}
	if (options_sounds.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(2)->construct();
		return true;
	}
	// On no button-mouse overlap
	return false;
}

// W_Inventory ---------------------------------------------------------------------------------------

W_Inventory::W_Inventory(InputWidget* parent) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> INVENTORY_MENU_CONSTR = {
		{viewCenter + sf::Vector2f(0.0f, -300.0f),	sf::Vector2f(100.0f, 100.0f),	sf::Color::Transparent,		100,	"INVENTORY", sf::Color::White},
		{viewCenter + sf::Vector2f(-500.0f, 0.0f),	sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"PISTOL",		sf::Color::White},
		{viewCenter + sf::Vector2f(0.0f, 0.0f),		sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"SHOTGUN",		sf::Color::White},
		{viewCenter + sf::Vector2f(500.0f, 0.0f),	sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"RIFLE",		sf::Color::White},
		{viewCenter + sf::Vector2f(0.0f, 300.0f),	sf::Vector2f(200.0f, 100.0f),	sf::Color::White,			24,		"RETURN",		sf::Color::Black}
	};

	inventory_title.construct(INVENTORY_MENU_CONSTR[0]);
	item1_Button.construct(INVENTORY_MENU_CONSTR[1]);
	item2_Button.construct(INVENTORY_MENU_CONSTR[2]);
	item3_Button.construct(INVENTORY_MENU_CONSTR[3]);
	return_Button.construct(INVENTORY_MENU_CONSTR[4]);
}

void W_Inventory::windowUpdate()
{
	InputWidget::windowUpdate();
	inventory_title.setPos(viewCenter + sf::Vector2f(0.0f, -300.0f));
	item1_Button.setPos(viewCenter + sf::Vector2f(-500.0f, 0.0f));
	item2_Button.setPos(viewCenter + sf::Vector2f(0.0f, 0.0f));
	item3_Button.setPos(viewCenter + sf::Vector2f(500.0f, 0.0f));
	return_Button.setPos(viewCenter + sf::Vector2f(0.0f, 300.0f));
}

InputWidget* W_Inventory::getWidgetAtIndex(const int& atIndex)
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

InputWidget* W_Inventory::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0:
		shapes = { &inventory_title, &item1_Button, &item2_Button, &item3_Button, &return_Button };
		break;
	default:
		shapes = { getWidgetAtIndex(widgetIndex) };
		break;
	}
	return getWidgetAtIndex(widgetIndex);
}

void W_Inventory::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);
}

bool W_Inventory::isMouseOver(const bool& checkForClick = false)
{
	if (item1_Button.isMouseOver(checkForClick) || item2_Button.isMouseOver(checkForClick) || item3_Button.isMouseOver(checkForClick))
	{
		if (checkForClick)
		{
			gameInstance->launchGame();
			return true;
		}
	}
	if (return_Button.isMouseOver(checkForClick))
	{
		if (checkForClick) input_esc();
		return true;
	}
	return false;
}


// W_LevelMenu ---------------------------------------------------------------------------------------

W_LevelMenu::W_LevelMenu(InputWidget* parent) : InputWidget(parent), inventory(this)
{
	const std::vector<ButtonConstruct> LEVEL_MENU_CONSTR = {
		{viewCenter + sf::Vector2f(0.0f, -300.0f),	sf::Vector2f(100.0f, 100.0f),	sf::Color::Transparent,		100,	"LEVEL SELECT", sf::Color::White},
		{viewCenter + sf::Vector2f(-500.0f, 0.0f),	sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"LEVEL 1",		sf::Color::White},
		{viewCenter + sf::Vector2f(0.0f, 0.0f),		sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"LEVEL 2",		sf::Color::White},
		{viewCenter + sf::Vector2f(500.0f, 0.0f),	sf::Vector2f(200.0f, 200.0f),	sf::Color(100, 100, 100),	24,		"LEVEL 3",		sf::Color::White},
		{viewCenter + sf::Vector2f(0.0f, 300.0f),	sf::Vector2f(200.0f, 100.0f),	sf::Color::White,			24,		"RETURN",		sf::Color::Black}
	};

	levelmenu_title.construct(LEVEL_MENU_CONSTR[0]);
	level1_Button.construct(LEVEL_MENU_CONSTR[1]);
	level2_Button.construct(LEVEL_MENU_CONSTR[2]);
	level3_Button.construct(LEVEL_MENU_CONSTR[3]);
	return_Button.construct(LEVEL_MENU_CONSTR[4]);
}

void W_LevelMenu::windowUpdate()
{
	InputWidget::windowUpdate();
	levelmenu_title.setPos(	viewCenter + sf::Vector2f(0.0f, -300.0f)	);
	level1_Button.setPos(	viewCenter + sf::Vector2f(-500.0f, 0.0f)	);
	level2_Button.setPos(	viewCenter + sf::Vector2f(0.0f, 0.0f)		);
	level3_Button.setPos(	viewCenter + sf::Vector2f(500.0f, 0.0f)		);
	return_Button.setPos(	viewCenter + sf::Vector2f(0.0f, 300.0f)		);
}

InputWidget* W_LevelMenu::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0:
		return this;
		break;
	case 1:
		return &inventory; // INVENTORY_MENU
		break;
	default:
		break;
	}
	return nullptr;
}

InputWidget* W_LevelMenu::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0:
		shapes = { &levelmenu_title, &level1_Button, &level2_Button, &level3_Button, &return_Button };
		break;
	default:
		shapes = { getWidgetAtIndex(widgetIndex) };
		break;
	}
	return getWidgetAtIndex(widgetIndex);
}

void W_LevelMenu::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);
}

bool W_LevelMenu::isMouseOver(const bool& checkForClick = false)
{
	if (level1_Button.isMouseOver(checkForClick) || level2_Button.isMouseOver(checkForClick) || level3_Button.isMouseOver(checkForClick))
	{
		if (checkForClick)
		{
			setWidgetIndex(1)->construct();
			//gameInstance->launchGame();
			return true;
		}
	}
	if (return_Button.isMouseOver(checkForClick))
	{
		if (checkForClick) input_esc();
			return true;
	}
	return false;
}

// W_Paused ---------------------------------------------------------------------------------------

W_Paused::W_Paused(InputWidget* parent) : InputWidget(parent), optionsMenu(this)
{
	const std::vector<ButtonConstruct> PAUSED_CONSTR = {
		{viewCenter + sf::Vector2f{ 0.0f, -300.0f },    sf::Vector2f{ 350.0f, 120.0f }, sf::Color::Transparent,   100, "PAUSE",											sf::Color::White},
		{viewCenter + sf::Vector2f{ 0.0f, 0.0f },       sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "RESUME",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0.0f, 150.0f },     sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "OPTIONS",													sf::Color::Black},
		{viewCenter + sf::Vector2f{ 0.0f, 300.0f },     sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "QUIT",														sf::Color::Black}
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

InputWidget* W_Paused::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
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

void W_Paused::windowUpdate()
{
	InputWidget::windowUpdate();
	pause_title.setPos(viewCenter + sf::Vector2f{ 0.0f, -300.0f });
	pause_resumeButton.setPos(viewCenter + sf::Vector2f{ 0.0f, 0.0f });
	pause_optionsButton.setPos(viewCenter + sf::Vector2f{ 0.0f, 150.0f });
	pause_quitButton.setPos(viewCenter + sf::Vector2f{ 0.0f, 300.0f });
}

bool W_Paused::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	if (pause_resumeButton.isMouseOver(checkForClick))
	{
		input_esc();
		return true;
	}
	if (pause_optionsButton.isMouseOver(checkForClick))
	{
		if (checkForClick) setWidgetIndex(1)->construct();
		return true;
	}
	if (pause_quitButton.isMouseOver(checkForClick))
	{
		if (checkForClick) gameInstance->setGameState(MENU_SCREEN);
		return true;
	}
	// On no button-mouse overlap
	return false;
}

bool W_Paused::input_esc()
{
	if (isChildActive())
		return getWidgetAtIndex(widgetIndex)->input_esc();
	if (parent != nullptr)
		parent->setWidgetIndex(0)->construct();
	return true;
}

// W_GameOver -------------------------------------------------------------------------------------

W_GameOver::W_GameOver(InputWidget* parent) : InputWidget(parent)
{
	const std::vector<ButtonConstruct> GAME_OVER_CONSTR = {
		{viewCenter + sf::Vector2f{ 0.0f, -300.0f },	sf::Vector2f{ 350.0f, 120.0f }, sf::Color::Transparent,   100, "GAME OVER",							sf::Color::White},
		{viewCenter + sf::Vector2f{ 0.0f, -200.0f },	sf::Vector2f{ 100.0f, 100.0f }, sf::Color::Transparent,   16, "Score: " + std::to_string(0),	sf::Color::White},
		{viewCenter + sf::Vector2f{ 0.0f, 0.0f },		sf::Vector2f{ 300.0f, 100.0f }, sf::Color::White,         24, "QUIT",									sf::Color::Black}
	};

	gameOver_title.construct(GAME_OVER_CONSTR[0]);
	gameOver_score.construct(GAME_OVER_CONSTR[1]);
	gameOver_quitButton.construct(GAME_OVER_CONSTR[2]);

	shapes = { &gameOver_title, &gameOver_score, &gameOver_quitButton };
}

void W_GameOver::windowUpdate()
{
	InputWidget::windowUpdate();

	gameOver_title.setPos(viewCenter + sf::Vector2f{ 0.0f, -300.0f });
	gameOver_score.setPos(viewCenter + sf::Vector2f{ 0.0f, -200.0f });
	gameOver_quitButton.setPos(viewCenter + sf::Vector2f{ 0.0f, 0.0f });
}

void W_GameOver::changeScore(const int& currScore = 0)
{
	gameOver_score.setText("Score: " + std::to_string(currScore));
}

bool W_GameOver::isMouseOver(const bool& checkForClick = false)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	if (gameOver_quitButton.isMouseOver(checkForClick))
	{
		gameInstance->setGameState(MENU_SCREEN);
		return true;
	}
	// On no button-mouse overlap
	return false;
}


// W_Gameplay -------------------------------------------------------------------------------------

W_Gameplay::W_Gameplay(InputWidget* parent) 
	: InputWidget(parent), pauseMenu(this), gameOverScreen(this), healthBar(10.0f, static_cast<float>(windowSize.x), 100.0f), background(sf::Quads, 4)
{
	// Load texture
	if (!backgroundTexture.loadFromFile("Content/Textures/cobblestone_mossy.png"))
	{
		std::cerr << "Error loading the background texture!" << std::endl;
	}
	backgroundTexture.setRepeated(true);

	// Done out
	std::cout << "- Constructed GameplayWidget" << std::endl;
}

void W_Gameplay::construct()
{
	InputWidget::construct();

	GameState gameState = gameInstance->getGameState();

	if (gameState < GAME_LAUNCHING) return getWidgetAtIndex(widgetIndex)->construct();

	if (gameState == GAME_LAUNCHING)
	{
		// Reset values to game start values
		gameInstance->startRound();
		healthBar.setMaxTime(TIMER_DEFAULT, true);
		// Add Gameplay objects to shapes vector to draw them
	}
	setWidgetIndex(0);
}

void W_Gameplay::windowUpdate()
{
	InputWidget::windowUpdate();

	// This creates the parallax effect: background moves as the sf::View moves
	constexpr float parallaxStrength = 1.0f;
	backgroundPos = viewCenter * parallaxStrength;  // Adjust this factor for stronger/weaker parallax

	// Background will cover the entire view area
	background[0].position = sf::Vector2f(viewCenter.x - viewSize.x / 2.0f, viewCenter.y - viewSize.y / 2.0f);
	background[1].position = sf::Vector2f(viewCenter.x + viewSize.x / 2.0f, viewCenter.y - viewSize.y / 2.0f);
	background[2].position = sf::Vector2f(viewCenter.x + viewSize.x / 2.0f, viewCenter.y + viewSize.y / 2.0f);
	background[3].position = sf::Vector2f(viewCenter.x - viewSize.x / 2.0f, viewCenter.y + viewSize.y / 2.0f);

	double textureOffsetX = fmod(backgroundPos.x * TILING_SCALE, backgroundTexture.getSize().x);
	double textureOffsetY = fmod(backgroundPos.y * TILING_SCALE, backgroundTexture.getSize().y);

	background[0].texCoords = sf::Vector2f(textureOffsetX, textureOffsetY);  // Top-left
	background[1].texCoords = sf::Vector2f(textureOffsetX + viewSize.x * TILING_SCALE, textureOffsetY);  // Top-right
	background[2].texCoords = sf::Vector2f(textureOffsetX + viewSize.x * TILING_SCALE, textureOffsetY + viewSize.y * TILING_SCALE);  // Bottom-right
	background[3].texCoords = sf::Vector2f(textureOffsetX, textureOffsetY + viewSize.y * TILING_SCALE);  // Bottom-left
}

InputWidget* W_Gameplay::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0: // SELF
		return this;
		break;
	case 1: // PAUSED
		return &pauseMenu;
		break;
	case 2: // GAME_OVER
		return &gameOverScreen;
		break;
	default:
		break;
	}
	return nullptr;
}

InputWidget* W_Gameplay::setWidgetIndex(const int& toIndex)
{
	shapes = { &background, &enemyManager, player, &projectileManager, &healthBar };

	switch (widgetIndex = toIndex)
	{
	case 0:
		gameInstance->setIsPaused(false);
		gameInstance->setGameState(IN_GAME);
		break;
	case 1:
		gameInstance->setIsPaused(true);
		gameInstance->setGameState(GAME_PAUSED);
		shapes.push_back(&pauseMenu);
		break;
	case 2:
		gameInstance->setIsPaused(true);
		gameInstance->setGameState(GAME_OVER);
		shapes.push_back(& gameOverScreen);
		break;
	default:
		shapes = {};
		break;
	}
	return getWidgetAtIndex(widgetIndex);
}

void W_Gameplay::lose()
{
	// Add GameOver Screen to shapes list
	setWidgetIndex(2)->construct();
	gameInstance->setIsPaused(true);
	gameOverScreen.changeScore(SaveGame::currentData.score);
	// Only save if higher than stored highscore
	if (SaveGame::currentData.score > SaveGame::storedData.score)
		SaveGame::saveData();
}

void W_Gameplay::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	// Make sure flashlight draws shader onto environment
	for (sf::Drawable* elem : shapes)
	{
		player->getFlashlight().drawOtherScene(elem);
	}
	

	// If Gameplay is UnPaused
	if (!gameInstance->getIsPaused())
	{
		enemyManager.tick(deltaTime);
		projectileManager.tick(deltaTime);
		collisionManager.tick(deltaTime);
		if (player->isDead())
			lose();
	}

	healthBar.tick(deltaTime);
}

bool W_Gameplay::input_esc()
{
	if (isChildActive())
		return getWidgetAtIndex(widgetIndex)->input_esc();
	// If no sub widget open, open optionsMenu
	setWidgetIndex(1)->construct(); 
	return true;
}

bool W_Gameplay::onMouseClickR(sf::Event* eventRef)
{
	if (gameInstance->getIsPaused())
		return false;

	return true;
}

bool W_Gameplay::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getWidgetAtIndex(widgetIndex)->isMouseOver(checkForClick);

	// Implement new click logic here (projectile direction etc)
	return false;
}

void W_Gameplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const sf::Drawable* elem : shapes)
	{
		if (elem == &background)
		{
			target.draw(background, &backgroundTexture);
			continue;
		}
		target.draw(*elem, states);
	}
}


