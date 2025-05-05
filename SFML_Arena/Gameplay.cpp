#pragma once

#include "Gameplay.h" // Own header file
#include "GameInstance.h"
#include "EntityManager.h"
#include "SoundManager.h"

W_Gameplay::W_Gameplay(InputWidget* parent)
	: InputWidget(parent),
	startDelay(START_DELAY), loadingPlaceholder(this),
	pauseMenu(this), gameOverScreen(this), inventoryScreen(this), levelUpScreen(this), background(this), fadeScreen(this),
	hud(gameInstance().getHud())
{

	const std::vector<RawButton> CONSTR = {
		{viewTL, viewSize, sf::Color::White, 0, "", sf::Color::Black, EAlignment::CENTER, EAlignment::CENTER},
		{viewTL, viewSize, sf::Color::Transparent, 500, "ARENA", sf::Color::Black, EAlignment::CENTER, EAlignment::CENTER}
	};

	fadeScreen.construct(CONSTR[0]);
	loadingPlaceholder.construct(CONSTR[1]);

	// Done out
	std::cout << "- Constructed GameplayWidget" << std::endl;
}

void W_Gameplay::construct()
{
	const GameState gameState = gameInstance().getGameState();

	if (gameState < GAME_LAUNCHING)
		return getWidgetAtIndex(widgetIndex)->construct();


	if (gameState == GAME_LAUNCHING)
	{
		InputWidget::construct();

		// Reset values to game start values
		setWidgetIndex(-1);
		gameInstance().startRound();
	}

}

InputWidget* W_Gameplay::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0: case -1: // SELF
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
	shapes = { &background, bottomRenderer, player, topRenderer, &hud };

	// Everything that comes before 'player' in shapes should be here
	flashlightAffectedDrawables = { &background.getVertexArray(), bottomRenderer };

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
	case -1:
	default:
		gameInstance().setIsPaused(false);
		gameInstance().setGameState(IN_GAME);
		flashlightAffectedDrawables.clear();
		shapes = { &fadeScreen, &loadingPlaceholder };
		return this;
		break;
	}

	shapes.push_back(&fadeScreen);

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
	loadingPlaceholder.tick(deltaTime);
	fadeScreen.tick(deltaTime);
	background.tick(deltaTime);

	Player* player = gameInstance().getPlayer();

	if (player->getInventory().getShouldLevelUp())
	{
		setWidgetIndex(3)->construct(); // Open level up screen
		player->getInventory().setShouldLevelUp(false); // Reset level up flag
		player->resetHealth(); // Reset health after leveling up
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
	// Indicates laoding or unfamiliar behavior, but could be not recognized by isChildActive call
	if (widgetIndex < 0)
		return false;

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
	for (const sf::Drawable* elem : shapes)
	{
		target.draw(*elem, states);
	}
}

void W_Gameplay::start_openAnim()
{
	switch (startAnimPhase)
	{
	case -1:
		startDelay.reset();
		fadeScreen.setFadeColor(sf::Color::White, sf::Color::Black, startDelay.getMaxValue());
		fadeScreen.startFade();
		startAnimPhase = 0;
		break;
	case 1:
		// Finally show hud and everything
		setWidgetIndex(0);
		hud.construct();

		fadeScreen.setFadeColor(sf::Color::Black, sf::Color::Transparent, SCREEN_FADE_DURATION);
		fadeScreen.startFade();
		break;
	default:
		break;
	}
	
}

void W_Gameplay::start_closeAnim()
{
	fadeScreen.setFadeColor(sf::Color::Transparent, sf::Color::Black, SCREEN_FADE_DURATION);
	fadeScreen.startFade();
}

void W_Gameplay::tick_openAnim(const float& deltaTime)
{
	switch (startAnimPhase)
	{
	case 0:
		startDelay.addValue(-deltaTime);
		if (startDelay.isEmpty())
		{
			startAnimPhase = 1;
			start_openAnim();
		}
		break;
	case 1:
		if (!fadeScreen.isFading())
		{
			startAnimPhase = -1;
			IWidgetAnimation::stopAnim();
		}
		break;
	default:
		stopAnim();
		break;
	}
}

void W_Gameplay::tick_closeAnim(const float&)
{
	if (!fadeScreen.isFading())
	{
		gameInstance().setGameState(MENU_SCREEN);
		IWidgetAnimation::stopAnim();
	}
}
