#pragma once

#include "Gameplay.h" // Own header file
#include "GameInstance.h"
#include "EntityManager.h"
#include "SoundManager.h"

W_Gameplay::W_Gameplay(InputWidget* parent)
	: InputWidget(parent),
	startDelay(START_DELAY), levelLoadingScreen(this),
	pauseMenu(this), gameOverScreen(this), inventoryScreen(this), levelUpScreen(this), background(this), fadeScreen(this),
	hud(gameInstance().getHud()), gameplayTimer(2.0f) // Have 2 seconds max so ms get accounted for
{
	// Done out
	std::cout << "- Constructed GameplayWidget" << std::endl;
}

void W_Gameplay::construct()
{
	const GameState gameState = gameInstance().getGameState();
	if (gameState == GAME_LAUNCHING)
	{
		// Reset values to game start values
		setWidgetIndex(-1)->construct();
		loadLevel(); // Loading correct level background
		levelUpScreen.reset(); // Resetting the Perk Trees on Game Launch
	}

	if (widgetIndex > 0)
		return;

	gameInstance().resetWindowName();
}

InputWidget* W_Gameplay::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case -1: // LEVEL LOADING SCREEN
		return &levelLoadingScreen;
		break;
	case 0: // SELF
		return this;
		break;
	case 1: // PAUSED
		return &pauseMenu;
		break;
	case 2: // GAME_OVER
		return &gameOverScreen;
		break;
	case 3: // LEVEL UP
		return &levelUpScreen;
		break;
	case 4: // INVENTORY
		return &levelUpScreen;
		break;
	default:
		break;
	}

	return nullptr;
}

InputWidget* W_Gameplay::setWidgetIndex(const int& toIndex)
{
	EntityManager& manager = EntityManager::getInstance();
	sf::Drawable* bottomRenderer = manager.getDrawableLayer(0);
	sf::Drawable* topRenderer = manager.getDrawableLayer(1);
	Player* player = gameInstance().getPlayer();

	if (!bottomRenderer || !topRenderer)
		return this;

	// DO NOT ADD BACKGROUND ELEMENT HERE, ALREADY ACCOUNTED FOR IN
	shapes = { &background, bottomRenderer, player, topRenderer, &hud };

	// Everything that comes before 'player' in shapes should be here
	flashlightAffectedDrawables = { &background.getVertexArray(), bottomRenderer };

	switch (widgetIndex = toIndex)
	{
	case 0: // SELF
		gameInstance().setGameState(IN_GAME);
		break;
	case 1: // PAUSED
		shapes.push_back(&pauseMenu);
		break;
	case 2: // GAME_OVER
		shapes.push_back(&gameOverScreen);
		break;
	case 3: // LEVEL UP
		shapes.push_back(&levelUpScreen);
		break;
	case 4: // INVENTORY
		shapes.push_back(&levelUpScreen);
		break;
	case -1:
	default:
		gameInstance().setIsPaused(true);
		flashlightAffectedDrawables.clear();
		shapes = { &levelLoadingScreen };
		return getWidgetAtIndex(widgetIndex);
	}

	shapes.push_back(&fadeScreen);

	return getWidgetAtIndex(widgetIndex);
}

void W_Gameplay::lose()
{
	// Add GameOver Screen to shapes list
	setWidgetIndex(2)->construct();
	// Save ón every lose, highscores get calculated internally
	SaveGame::saveData();
}

void W_Gameplay::loadLevel()
{
	unsigned int levelID = gameInstance().getSelectedLevel();
	loadLevel(levelID);
}

void W_Gameplay::loadLevel(const unsigned int& levelID)
{
	EBackgroundTexture levelBg = static_cast<EBackgroundTexture>(levelID);
	background.setBackgroundTexture(levelBg);
}

void W_Gameplay::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	background.tick(deltaTime);
	hud.tick(deltaTime);
	fadeScreen.tick(deltaTime);

	Player* player = gameInstance().getPlayer();
	player->tick(deltaTime);
	
	// If Gameplay is UnPaused
	if (!gameInstance().getIsPaused())
	{
		EntityManager::getInstance().tick(deltaTime); // Update all entities
		CollisionManager::getInstance().tick(deltaTime); // Update all collisions

		if (player->isDead())
		{
			lose(); // Check if player is dead and call lose if so
		}

		Inventory& inv = player->getInventory();
		if (inv.getNumQueuedLevelUps() > 0)
		{
			menu_levelUp();
			inv.removeQueuedLevelUp(); // Reset level up flag
		}
		
		// Count seconds for highscore and unlock purposes
		tick_countSeconds(deltaTime);
	}

	// Make sure flashlight draws shader onto environment
	Flashlight& flashlight = player->getFlashlight();

	for (sf::Drawable* shape : flashlightAffectedDrawables)
	{
		flashlight.drawOtherScene(shape);
	}
}

bool W_Gameplay::onKeyEscape()
{
	// Indicates loading or unfamiliar behavior, but could be not recognized by isChildActive call
	if (widgetIndex < 0)
		return false;

	if (isChildActive())
		return getActiveChild()->onKeyEscape();

	// If no sub widget open, open optionsMenu
	setWidgetIndex(1)->construct();
	return true;
}

bool W_Gameplay::onKeyTab()
{
	if (isChildActive())
		return getActiveChild()->onKeyTab();

	SoundManager& soundManager = SoundManager::getInstance();
	soundManager.play(soundManager.getSound_ReturnClick(), ESoundEnv::UI);

	setWidgetIndex(4)->construct();
	return true;
}

bool W_Gameplay::onLostFocus()
{
	if (isChildActive())
		return getActiveChild()->onLostFocus();

	// If no sub widget open, open optionsMenu
	setWidgetIndex(1)->construct();
	return true;
}

bool W_Gameplay::onMouseClickR(sf::Event* eventRef)
{
	if (gameInstance().getIsPaused())
		return false;

	return true;
}

void W_Gameplay::tick_countSeconds(const float& deltaTime)
{
	gameplayTimer.addValue(-deltaTime);
	if (gameplayTimer.getValue() <= 1.0f)
	{
		SaveGame::currentData.secondsPlayed++;
		gameplayTimer.addValue(1.0f);
	}
}

void W_Gameplay::menu_levelUp()
{
	setWidgetIndex(3)->construct(); // Open level up screen
	// Player level Up sound
	SoundManager& soundManager = SoundManager::getInstance();
	soundManager.play(soundManager.getSound_LevelUp(), ESoundEnv::UI);
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
		target.draw(*elem, states);
	}
}


sf::Vector2f W_Gameplay::getTickCorrection() const
{
	return widgetOffset;
}

void W_Gameplay::start_openAnim()
{
	// Technical stuff for Gameplay
	gameInstance().startRound();
	// Make loading Screen visible and start HUD animations (best ones)
	setWidgetIndex(0)->construct();
	hud.construct();

	fadeScreen.reset(ColorColor(sf::Color::Black, sf::Color::Transparent), SCREEN_FADE_DURATION, easing::expo::out);
}

void W_Gameplay::tick_openAnim(const float& deltaTime)
{
	fadeScreen.fade(deltaTime);

	// Only stop anim if loading is complete
	if (fadeScreen.isFading())
		return;
	
	stopAnim(OPEN_ANIM);
}

void W_Gameplay::start_closeAnim()
{
	fadeScreen.reset(ColorColor(sf::Color::Transparent, sf::Color::Black), SCREEN_FADE_DURATION);
}

void W_Gameplay::tick_closeAnim(const float& deltaTime)
{
	fadeScreen.fade(deltaTime);

	// Only stop anim if fading is complete
	if (fadeScreen.isFading())
		return;

	gameInstance().setGameState(MENU_SCREEN);
	stopAnim(CLOSE_ANIM);
}