#pragma once

#include "Gameplay.h" // Own header file
#include "GameInstance.h"
#include "EntityManager.h"
#include "SoundManager.h"

W_Gameplay::W_Gameplay(InputWidget* parent)
	: InputWidget(parent),
	pauseMenu(this), gameOverScreen(this), inventoryScreen(this), levelUpScreen(this), background(this),
	hud(gameInstance().getHud())
{
	// Done out
	std::cout << "- Constructed GameplayWidget" << std::endl;
}

void W_Gameplay::construct()
{
	InputWidget::construct();

	GameState gameState = gameInstance().getGameState();

	if (gameState < GAME_LAUNCHING)
		return getWidgetAtIndex(widgetIndex)->construct();

	if (gameState == GAME_LAUNCHING)
	{
		// Reset values to game start values
		hud.construct();
		gameInstance().startRound();
	}

	setWidgetIndex(0);
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
	case 3: // LEVEL UP
		return &levelUpScreen;
		break;
	case 4: // INVENTPRY
		return &inventoryScreen;
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
	shapes = { bottomRenderer, player, topRenderer, &hud };

	switch (widgetIndex = toIndex)
	{
	case 0: // SELF
		gameInstance().setIsPaused(false);
		gameInstance().setGameState(IN_GAME);
		break;
	case 1: // PAUSED
		gameInstance().setIsPaused(true);
		gameInstance().setGameState(GAME_PAUSED);
		shapes.push_back(&pauseMenu);
		break;
	case 2: // GAME_OVER
		gameInstance().setIsPaused(true);
		gameInstance().setGameState(GAME_OVER);
		shapes.push_back(&gameOverScreen);
		break;
	case 3: // LEVEL UP
		gameInstance().setIsPaused(true);
		gameInstance().setGameState(GAME_PAUSED);
		shapes.push_back(&levelUpScreen);
		break;
	case 4: // INVENTORY
		gameInstance().setIsPaused(true);
		gameInstance().setGameState(GAME_PAUSED);
		shapes.push_back(&inventoryScreen);
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
	// Only save if higher than stored highscore
	if (SaveGame::currentData.score > SaveGame::storedData.score)
	{
		SaveGame::saveData();
	}
}

void W_Gameplay::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
	hud.tick(deltaTime);
	background.tick(deltaTime);

	Player* player = gameInstance().getPlayer();

	if (player->getInventory().getShouldLevelUp())
	{
		setWidgetIndex(3)->construct(); // Open level up screen
		player->getInventory().setShouldLevelUp(false); // Reset level up flag
		player->setHealth(player->getMaxHealth()); // Reset health after leveling up
	}

	// If Gameplay is UnPaused
	if (!gameInstance().getIsPaused())
	{
		EntityManager::getInstance().tick(deltaTime); // Update all entities
		CollisionManager::getInstance().tick(deltaTime); // Update all collisions
		if (player->isDead()) 
			lose(); // Check if player is dead and call lose if so
	}

	if (isChildActive())
		getActiveChild()->tick(deltaTime);

	// Make sure flashlight draws shader onto environment
	Flashlight& flashlight = player->getFlashlight();

	flashlight.drawOtherScene(background.getVertexArray());

	for (sf::Drawable* elem : shapes)
	{
		flashlight.drawOtherScene(elem);
	}
}

bool W_Gameplay::onKeyEscape()
{
	if (isChildActive())
		return getActiveChild()->onKeyEscape();

	// If no sub widget open, open optionsMenu
	setWidgetIndex(1)->construct();
	return true;
}

bool W_Gameplay::onKeyTab()
{
	// If no sub widget open, open optionsMenu
	SoundManager& soundManager = SoundManager::getInstance();
	soundManager.play(soundManager.getSound_ReturnClick());

	if (isChildActive())
		return getActiveChild()->onKeyTab();

	setWidgetIndex(4)->construct();
	return true;
}

bool W_Gameplay::onMouseClickR(sf::Event* eventRef)
{
	if (gameInstance().getIsPaused())
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
	background.draw(target, states);

	for (const sf::Drawable* elem : shapes)
	{
		target.draw(*elem, states);
	}
}