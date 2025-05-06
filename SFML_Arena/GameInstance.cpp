#pragma once
#include <iostream>

#include "GameInstance.h"
#include "Widgets.h"
#include "EntityManager.h"
#include "SoundManager.h"
#include "Player.h"

float GI_Arena::globalTime = 0.0f;

GI_Arena::GI_Arena()
{
	std::cout << "### Creating viewport..." << std::endl;
	const sf::VideoMode DESKTOP = sf::VideoMode::getDesktopMode();

	const sf::Uint32 WINDOW_STYLE = (bDevMode ? sf::Style::Default : sf::Style::Fullscreen);
	window = std::make_unique<sf::RenderWindow>(DESKTOP, WINDOW_NAME, WINDOW_STYLE);

	window->setFramerateLimit(MAX_FPS);
	window->setVerticalSyncEnabled(bUseVSync);

	std::cout << "- RenderWindow created" << std::endl;
	sf::Vector2f desktopSize = { static_cast<float>(DESKTOP.width), static_cast<float>(DESKTOP.height) };
	view = std::make_unique<sf::View>(desktopSize / 2.0f, desktopSize);
	prevCamPos = widgetOffset = view->getCenter();
	std::cout << "- View created" << std::endl;
	window->setView(*view);
	std::cout << "- View attached" << std::endl;

	std::cout << "Initiated viewport\n" << std::endl;
}

bool GI_Arena::initWidgets()
{
	// Only execute this method once
	static bool bInitiatedWidgets = false;

	if (bInitiatedWidgets)
		return false;

	widgets.clear();
	std::cout << "### Initiating widgets..." << std::endl;
	try { // Adding base widgets with their nullptr parent HERE!
		widgets.push_back(std::make_shared<W_LoadingScreen>(nullptr));	// LoadingScreen = 0
		widgets.push_back(std::make_shared<W_MainMenu>(nullptr));		// MainMenu = 1
		widgets.push_back(std::make_shared<W_Gameplay>(nullptr));		// Gameplay = 2
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		widgets.clear(); // Cleanup in case of partial initialization
		return false;
	}

	// Console log
	std::cout << "Initiated widgets\n" << std::endl;
	bInitiatedWidgets = true;
	return true;
}

Player* GI_Arena::validPlayer()
{
	if (player)
	{
		return player.get();
	}

	player = std::make_unique<Player>();
	return player.get();
}

void GI_Arena::start()
{
	SaveGame::loadSavedData();

	IDrawableShapes::initValues();
	std::cout << "Viewport values initialized\n" << std::endl;

	validPlayer();

	initWidgets();
	
	std::cout << "\n### Starting Game ###\n" << std::endl;

	tickLoop();
}

void GI_Arena::tickLoop()
{
	// Main Game Loop
	while (window->isOpen())
	{
		// Debug time measuring only TEMPORARY, disbale if needed
		//sf::Clock debugClock;
		preTick();
		// Calculate deltaTime for time corrected physics
		const float deltaTime = clock.restart().asSeconds();
		tick(deltaTime);
		postTick();
		//std::cout << "tick loop time: " << debugClock.restart().asMilliseconds() << "ms" << std::endl; // Debug
	}
}

void GI_Arena::correctWidget()
{
	// If GameState changed in earlier loop, construct new activeMenu;
	static GameState oldGS = QUIT;

	// On no change, don't call
	if (gameState == oldGS) 
		return;

	switch (oldGS = gameState)
	{
	case LOADING_SCREEN:
		activeMenu = widgets[0];
		break;
	case MENU_SCREEN: // MAIN_MENU
		activeMenu = widgets[1];
		break;
	case GAME_PAUSED: case GAME_OVER: case GAME_LAUNCHING: case IN_GAME: // GAMEPLAY
		activeMenu = widgets[2];
		break;
	case QUIT: // QUIT GAME
		activeMenu = nullptr;
		break;
	default: // KEEP
		break;
	}

	// If new widget not valid, close game / end loop
	if (activeMenu == nullptr)
	{
		window->close();
		return;
	}

	activeMenu->construct();
}

void GI_Arena::launchGame()
{
	if (gameState == GAME_LAUNCHING)
		return;

	setGameState(GAME_LAUNCHING);
	SaveGame::currentData.clear();
}

void GI_Arena::startRound()
{
	// Only activate Gameplay phase when at least launch phase
	if (gameState != GAME_LAUNCHING)
		return;

	// Resetting gameplay aspects
	EntityManager::getInstance().deleteAll();
	player->spawn();
	resetViewPos();
	// Set gameState to IN_GAME
	setGameState(IN_GAME);
}

void GI_Arena::preTick()
{
	prevCamPos = view->getCenter();
	SoundManager::getInstance().cleanUp();
}

void GI_Arena::tick(const float& deltaTime)
{
	// Time variables
	globalTime += deltaTime;

	// Update sf::View based on rescaling or ...
	tick_view(deltaTime);

	// Event management
	sf::Event event;
	while (window->pollEvent(event) && gameState > QUIT)
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
			break;
		}
		// Player gets to distribute the events and inputs
		player->handleEvent(&event);
	}

	//player->tick(deltaTime); TODO remove line
	correctWidget();

	// Tick active Environment
	if (activeMenu != nullptr)
	{
		activeMenu->tick(deltaTime);
	}
}

void GI_Arena::postTick()
{
	// Draw new Menu to screen through GameInstance
	updateScreen();
}

void GI_Arena::setViewPos(const sf::Vector2f& newPos)
{
	view->setCenter(newPos);
	window->setView(*view);
}

// ReCenter sf::View position on current Player position and set prevCamPos
void GI_Arena::resetViewPos()
{
	setViewPos(getPlayer()->getPosition());
	prevCamPos = view->getCenter();
}

sf::Vector2f GI_Arena::getWidgetOffset() const
{
	return widgetOffset;
}

// Adjust the camera position based on velocity and distance to target (Player + Mouse Influence)
void GI_Arena::tick_view(const float& deltaTime)
{
	constexpr float SPRING_STRENGTH = 2.5f;				// Higher = Quicker follow
	constexpr float DAMPING_COEFFICIENT = 4.0f;			// Higher = More Resistance
	constexpr float MAX_DISTANCE = 100.0f;				// Maximum allowed mouse influence
	constexpr float WIDGET_LERP_ALPHA = 0.1f;            // Lower value = slower, snappier response
	constexpr float MIN_DELTA = 0.001f;                  // Minimum delta time to prevent extreme forces

	// Clamp deltaTime to prevent extreme forces on first frame or lag spikes
	float clampedDelta = std::max(deltaTime, MIN_DELTA);
	if (clampedDelta > 0.1f) clampedDelta = 0.1f;        // Also prevent too large steps

	bool bIsGameOver = getGameState() == GAME_OVER;

	// Get current camera, player, and mouse positions
	const sf::Vector2f camPos = view->getCenter();
	const sf::Vector2f playerPos = getPlayer()->getPosition();
	const sf::Vector2f mousePos = (bIsGameOver ? playerPos : getMousePos());

	static sf::Vector2f mouseOffset;

	// Calculate the mouse offset relative to the player
	if (bWidgetParallax || !getIsPaused() || bIsGameOver)
	{
		mouseOffset = mousePos - playerPos;
		float offsetLength = std::sqrt(mouseOffset.x * mouseOffset.x + mouseOffset.y * mouseOffset.y);
		// Clamp the mouse offset so it doesn't exceed MAX_DISTANCE
		if (offsetLength > MAX_DISTANCE)
		{
			mouseOffset = (mouseOffset / offsetLength) * MAX_DISTANCE;
		}
	}

	// Determine the new target position: player's position plus the clamped mouse offset
	sf::Vector2f targetPos = playerPos + mouseOffset;

	// Compute the distance from the current camera position to the target position
	sf::Vector2f distance = camPos - targetPos;

	// Calculate the spring force using the distance
	sf::Vector2f springForce = -SPRING_STRENGTH * distance;

	// Compute the camera's velocity based on the previous camera position
	sf::Vector2f velocity = (camPos - prevCamPos) / clampedDelta;

	// Calculate the damping force using the velocity
	sf::Vector2f dampingForce = -DAMPING_COEFFICIENT * velocity;

	// Combine forces to determine the total force applied to the camera
	sf::Vector2f totalForce = springForce + dampingForce;

	// Update the camera position by applying the total force scaled by deltaTime
	sf::Vector2f newCamPos = camPos + (totalForce * clampedDelta);
	setViewPos(newCamPos);

	const sf::Vector2f viewCenter = view->getCenter();
	if (bWidgetParallax)
	{
		// Lerp previous widgetOffset towards the new inverted offset
		const sf::Vector2f newCenter = viewCenter - (mouseOffset * 0.1f);
		widgetOffset = lerp(widgetOffset, newCenter, WIDGET_LERP_ALPHA);
	}
	else
	{
		widgetOffset = viewCenter;
	}

	IDrawableShapes::updateValues();
}

void GI_Arena::updateScreen()
{
	// Clear viewport for new draw
	window->clear(sf::Color::Black);
	// Draw all Drawables from shapes vector
	if (activeMenu != nullptr) window->draw(*activeMenu);
	// Display Draw changes
	window->display();
}

// TODO: Rewrite the GameState logic, handling, disconnect from Pausing, reduce control etc
void GI_Arena::setGameState(const GameState& newGS)
{
	// Refuse to acknowledge "change" when nothing changed
	if (newGS == gameState)
		return; // eg when opening Gameplay and animation needs IN_GAME

	// TODO needs oversight
	std::cout << "Setting gameState to = " << static_cast<int>(newGS) << std::endl;

	switch (gameState = newGS)
	{
	case IN_GAME:
		setIsPaused(false);
		break;
	case GAME_PAUSED:
	case GAME_OVER:
		setIsPaused(true);
		break;
	default:
		break;
	}
}

void GI_Arena::resetWindowName()
{
	window->setTitle(WINDOW_NAME);
}

void GI_Arena::modWindowName(const std::string& suffix)
{
	const std::string windowTitle = WINDOW_NAME + suffix;
	window->setTitle(windowTitle);
}

Player* GI_Arena::getPlayer()
{
	if (player)
	{
		return player.get();
	}

	return validPlayer();
}

bool GI_Arena::handleEvent(sf::Event* eventRef)
{
	if (activeMenu)
		return activeMenu->handleEvent(eventRef);

	return false;
}