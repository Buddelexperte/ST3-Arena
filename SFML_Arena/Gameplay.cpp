#pragma once

#include "Gameplay.h" // Own header file
#include "GameInstance.h"
#include "EntityManager.h"
#include "SoundManager.h"

W_Gameplay::W_Gameplay(InputWidget* parent)
	: InputWidget(parent),
	startDelay(START_DELAY), loadingTitle(this),
	pauseMenu(this), gameOverScreen(this), inventoryScreen(this), levelUpScreen(this), background(this), fadeScreen(this),
	hud(gameInstance().getHud())
{

	const std::vector<RawButton> CONSTR = {
		{viewTL, viewSize, sf::Color::White, 0, "", sf::Color::Black, EAlignment::CENTER, EAlignment::CENTER},
		{viewTL, viewSize, sf::Color::Transparent, 500, "ARENA", sf::Color::White, EAlignment::CENTER, EAlignment::CENTER}
	};

	fadeScreen.construct(CONSTR[0]);
	loadingTitle.construct(CONSTR[1]);

	// Done out
	std::cout << "- Constructed GameplayWidget" << std::endl;
}

void W_Gameplay::construct()
{
	const GameState gameState = gameInstance().getGameState();
	if (gameState == GAME_LAUNCHING)
	{
		// Reset values to game start values
		playAnim(EAnimation::OPEN_ANIM);
		gameInstance().startRound();
	}

	if (isChildActive())
		return;

	gameInstance().resetWindowName();
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
		shapes.push_back(&inventoryScreen);
		break;
	case -1:
	default:
		gameInstance().setIsPaused(true);
		flashlightAffectedDrawables.clear();
		shapes = { &fadeScreen, &loadingTitle };
		return this;
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
	loadingTitle.tick(deltaTime);
	fadeScreen.tick(deltaTime);
	background.tick(deltaTime);

	Player* player = gameInstance().getPlayer();

	player->tick(deltaTime);

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
	soundManager.play(soundManager.getSound_ReturnClick(), ESoundEnv::UI);

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


sf::Vector2f W_Gameplay::getCorrectTickCorrection() const
{
	return widgetOffset;
}

void W_Gameplay::start_openAnim()
{
	switch (startAnimPhase)
	{
	case -1:
		setWidgetIndex(-1);

		startDelay.setMaxValue(START_DELAY);
		startDelay.reset();

		loadingTitle.setFadeColor(
			ColorColor(sf::Color::Black, sf::Color::Black), 
			ColorColor(sf::Color::Black, sf::Color::White), 
			startDelay.getMaxValue() / 2.0f, 
			easing::expo::out
		);
		loadingTitle.startFade();
		startAnimPhase = 0;
		break;
	case 0:
		loadingTitle.setFadeColor(
			ColorColor(sf::Color::Black, sf::Color::Black),
			ColorColor(sf::Color::White, sf::Color::Black),
			startDelay.getMaxValue() / 2.0f, 
			easing::cubic::in
		);
		loadingTitle.startFade();
		break;
	case 1:
		setWidgetIndex(0);
		hud.construct();

		fadeScreen.setFadeColor(
			ColorColor(sf::Color::Black, sf::Color::Transparent), 
			SCREEN_FADE_DURATION, 
			easing::cubic::out
		);
		fadeScreen.startFade();
		break;
	default:
		break;
	}
	
}

void W_Gameplay::start_closeAnim()
{
	fadeScreen.setFadeColor(ColorColor(sf::Color::Transparent, sf::Color::Black), SCREEN_FADE_DURATION);
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
			break;
		}
		if (!loadingTitle.isFading())
		{
			start_openAnim();
		}
		break;
	case 1:
		if (!fadeScreen.isFading())
		{
			startAnimPhase = -1;
			stopAnim(OPEN_ANIM);
		}
		break;
	default:
		stopAnim(OPEN_ANIM);
		break;
	}
}

void W_Gameplay::tick_closeAnim(const float&)
{
	if (!fadeScreen.isFading())
	{
		gameInstance().setGameState(MENU_SCREEN);
		stopAnim(CLOSE_ANIM);
	}
}
