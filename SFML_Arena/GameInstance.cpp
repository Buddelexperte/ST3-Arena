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
	const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	window = std::make_unique<sf::RenderWindow>(desktop, "SFML_Arena", sf::Style::Fullscreen);

	// Only use for crash heavy debug !
	//window = std::make_unique<sf::RenderWindow>(desktop, "SFML_Arena", sf::Style::Titlebar | sf::Style::Default);

	window->setFramerateLimit(MAX_FPS);
	window->setVerticalSyncEnabled(bUseVSync);

	std::cout << "RenderWindow created." << std::endl;
	sf::Vector2f desktopSize = { static_cast<float>(desktop.width), static_cast<float>(desktop.height) };
	view = std::make_unique<sf::View>(desktopSize / 2.0f, desktopSize);
	prevCamPos = widgetOffset = view->getCenter();
	std::cout << "View created" << std::endl;
	window->setView(*view);
	std::cout << "View attached" << std::endl;

	SaveGame::loadSavedData();
}

bool GI_Arena::initWidgets()
{
	// Only execute this method once
	static bool didOnce = false;

	if (didOnce)
		return false;
	didOnce = true;

	widgets.clear();
	std::cout << "Initiating widgets..." << std::endl;
	try { // Adding base widgets with their nullptr parent HERE!
		widgets.push_back(std::make_shared<W_LoadingScreen>(nullptr)); // LoadingScreen = 0
		widgets.push_back(std::make_shared<W_MainMenu>(nullptr)); // MainMenu = 1
		widgets.push_back(std::make_shared<W_Gameplay>(nullptr)); // Gameplay = 2
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		widgets.clear(); // Cleanup in case of partial initialization
		return false;
	}

	// Console log
	std::cout << "Initiated widgets" << std::endl;
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
	IDrawableShapes::initValues();

	validPlayer();
	std::cout << "Player created" << std::endl;

	initWidgets();
	std::cout << "Widgets created" << std::endl;
	
	waitForFirstRender();

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
	if (gameState >= GAME_LAUNCHING)
		return;

	gameState = GAME_LAUNCHING;
	SaveGame::currentData.clear();
}

void GI_Arena::startRound()
{
	// Only activate Gameplay phase when at least launch phase
	if (gameState < GAME_LAUNCHING)
		return;

	// Resetting gameplay aspects
	EntityManager::getInstance().deleteAll();
	player->spawn();
	resetViewPos();
	// Set gameState to IN_GAME
	setGameState(IN_GAME);
}

void GI_Arena::waitForFirstRender()
{
	// First manual draw
	updateScreen();

	sf::sleep(sf::milliseconds(1000)); // Sleep ~1 frame
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

	// Tick compound actors
	player->tick(deltaTime);
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
	constexpr float SPRING_STRENGTH = 2.5f;             // Higher = Quicker follow
	constexpr float DAMPING_COEFFICIENT = 4.0f;           // Higher = More Resistance
	constexpr float MAX_DISTANCE = 100.0f;                // Maximum allowed mouse influence

	// A factor for widgetOffset interpolation (tweak for less aggressive movement)
	constexpr float WIDGET_LERP_ALPHA = 0.5f;             // Lower value = slower, snappier response

	// Get current camera, player, and mouse positions
	const sf::Vector2f camPos = view->getCenter();
	const sf::Vector2f playerPos = getPlayer()->getPosition();
	const sf::Vector2f mousePos = getMousePos();

	static sf::Vector2f mouseOffset;

	// Calculate the mouse offset relative to the player
	if (bWidgetParallax || !getIsPaused())
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
	sf::Vector2f velocity = camPos - prevCamPos;

	// Calculate the damping force using the velocity
	sf::Vector2f dampingForce = -DAMPING_COEFFICIENT * velocity;

	// Combine forces to determine the total force applied to the camera
	sf::Vector2f totalForce = springForce + dampingForce;

	// Update the camera position by applying the total force scaled by deltaTime
	sf::Vector2f newCamPos = camPos + (totalForce * deltaTime);
	setViewPos(newCamPos);

	if (bWidgetParallax)
	{
		// Instead of scaling newCamPos directly, calculate the widget offset relative to the player's position.
		// This sets widgetOffset to be halfway between playerPos and newCamPos.
		widgetOffset = lerp(playerPos + (newCamPos - playerPos) * 0.2f, view->getCenter(), WIDGET_LERP_ALPHA);
	}
	else
	{
		widgetOffset = view->getCenter();
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

void GI_Arena::setGameState(const GameState& newGS)
{
	gameState = newGS;
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