#pragma once

#include "Gameplay.h" // Own header file
#include "GameInstance.h"
#include "EntityManager.h"
#include "SoundManager.h"

W_Gameplay::W_Gameplay(InputWidget* parent)
	: InputWidget(parent),
	pauseMenu(this), gameOverScreen(this), inventoryScreen(this), hud(gameInstance().getHud()), levelUpScreen(this),
	background(sf::Quads, 4)
{
	// Load texture
	if (!backgroundTexture.loadFromFile("Content/Textures/cobblestone_mossy.png"))
	{
		std::cerr << "Error loading the background texture!" << std::endl;
	}
	backgroundTexture.setRepeated(true);

	// Done out
	std::cout << "- Constructed GameplayWidget" << std::endl;
	tick_background(0.0f);
	window->draw(background, &backgroundTexture);
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

void W_Gameplay::tick_background(const float& deltaTime)
{
	// This creates the parallax effect: background moves as the sf::View moves
	constexpr float parallaxStrength = 1.0f;
	backgroundPos = viewCenter * parallaxStrength; // Adjust this factor for stronger/weaker parallax

	// Background will cover the entire view area
	background[0].position = sf::Vector2f(viewCenter.x - viewSize.x / 2.0f, viewCenter.y - viewSize.y / 2.0f);
	background[1].position = sf::Vector2f(viewCenter.x + viewSize.x / 2.0f, viewCenter.y - viewSize.y / 2.0f);
	background[2].position = sf::Vector2f(viewCenter.x + viewSize.x / 2.0f, viewCenter.y + viewSize.y / 2.0f);
	background[3].position = sf::Vector2f(viewCenter.x - viewSize.x / 2.0f, viewCenter.y + viewSize.y / 2.0f);

	float textureOffsetX = static_cast<float>(fmod(backgroundPos.x * TILING_SCALE, backgroundTexture.getSize().x));
	float textureOffsetY = static_cast<float>(fmod(backgroundPos.y * TILING_SCALE, backgroundTexture.getSize().y));

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

	shapes = { &background, bottomRenderer, player, topRenderer, &hud };

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
	tick_background(deltaTime);

	Player* player = gameInstance().getPlayer();

	if (player->getInventory().getShouldLevelUp())
	{
		setWidgetIndex(3)->construct();
		player->getInventory().setShouldLevelUp(false);
	}

	// If Gameplay is UnPaused
	if (!gameInstance().getIsPaused())
	{
		EntityManager::getInstance().tick(deltaTime);
		CollisionManager::getInstance().tick(deltaTime);
		if (player->isDead())
			lose();
	}

	if (isChildActive())
		getActiveChild()->tick(deltaTime);

	// Make sure flashlight draws shader onto environment
	for (sf::Drawable* elem : shapes)
	{
		player->getFlashlight().drawOtherScene(elem);
	}
}

bool W_Gameplay::onKeyEscape()
{
	if (isChildActive())
		return getActiveChild()->onKeyEscape();

	// If no sub widget open, open optionsMenu
	SoundManager& soundManager = SoundManager::getInstance();
	soundManager.play(soundManager.getSound_ReturnClick());
	setWidgetIndex(1)->construct();
	return true;
}

bool W_Gameplay::onKeyTab()
{
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