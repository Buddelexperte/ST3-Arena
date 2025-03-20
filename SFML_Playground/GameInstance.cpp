#pragma once

#include "GameInstance.h"
#include "Widgets.h"

// Game Instance Code

GI_Arena::GI_Arena()
{
	const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	//window = new sf::RenderWindow(desktop, "SFML_Arena", sf::Style::Fullscreen);

	// Only use for crash heavy debug !
	window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "SFML_Arena", sf::Style::Titlebar | sf::Style::Default);

	std::cout << "RenderWindow created." << std::endl;
	sf::Vector2f desktopSize = { static_cast<float>(desktop.width), static_cast<float>(desktop.height) };
	view = new sf::View(desktopSize / 2.0f, desktopSize);
	prevCamPos = view->getCenter();
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
		widgets.push_back(std::make_shared<W_MainMenu>(nullptr)); // MainMenu = 0
		widgets.push_back(std::make_shared<W_Gameplay>(nullptr)); // Gameplay = 1
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		widgets.clear(); // Cleanup in case of partial initialization
		return false;
	}

	// Console log
	std::cout << "Initiated widgets" << std::endl;

	// Apply correct widget
	correctWidget();
	return true;
}

Player* GI_Arena::makePlayer()
{
	if (player)
	{
		return player;
	}

	player = new Player(nullptr);
	return player;
}

void GI_Arena::start()
{
	makePlayer();
	std::cout << "Player created" << std::endl;

	initWidgets();

	std::cout << "\n### Starting Game ###\n" << std::endl;

	tickLoop();
}

void GI_Arena::correctWidget()
{
	// If GameState changed in earlier loop, construct new activeMenu;
	static GameState oldGS = QUIT;

	if (gameState == oldGS) return;

	switch (oldGS = gameState)
	{
	case MENU_SCREEN: // MAIN_MENU
		activeMenu = widgets[0];
		break;
	case GAME_PAUSED: case GAME_OVER: case GAME_LAUNCHING: case IN_GAME: // GAMEPLAY
		activeMenu = widgets[1];
		break;
	case QUIT: // QUIT GAME
		activeMenu = nullptr;
		break;
	default: // KEEP
		break;
	}

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
	if (gameState < GAME_LAUNCHING)
		return;

	EnemyManager::getInstance().deleteAll();
	gameState = IN_GAME;
	player->spawn();
	resetViewPos();
}

void GI_Arena::preTick()
{
	prevCamPos = view->getCenter();
	SoundManager::getInstance().cleanUp();
}

void GI_Arena::tick(const float& deltaTime)
{
	tickView(deltaTime);

	sf::Event event;
	while (window->pollEvent(event) && gameState > QUIT)
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
			break;
		}
		player->handleEvent(&event);	// Mouse buttons getting pressed down
	}

	// Non-event inputs
	player->tick(deltaTime);
	activeMenu->tick(deltaTime);
}

void GI_Arena::postTick()
{
	correctWidget();

	// Draw new Menu to screen through GameInstance
	updateScreen();
}

void GI_Arena::tickLoop()
{
	// Main Game Loop
	while (window->isOpen())
	{
		preTick();
		// Calculate deltaTime for time corrected physics
		const float deltaTime = clock.restart().asSeconds();
		tick(deltaTime);
		postTick();
	}
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

// Adjust the camera position based on velocity and distance to player (Spring-Damp-System)
void GI_Arena::tickView(const float& deltaTime)
{
	constexpr float SPRING_STRENGTH = 2.5f; // Higher = Quicker follow
	constexpr float DAMPING_COEFFICIENT = 4.0f; // Higher = More Resistance

	// Get current camera and player positions
	const sf::Vector2f& camPos = view->getCenter();
	const sf::Vector2f& playerPos = getPlayer()->getPosition();

	// Calculate distance between both positions (x and y)
	sf::Vector2f distance = camPos - playerPos;
	// Calculate Spring Force (Using distance to target)
	sf::Vector2f springForce = -SPRING_STRENGTH * distance;

	// Calculate velocity (Using old and new position)
	sf::Vector2f velocity = camPos - prevCamPos;
	// Calculate Damping Force (Using velocity)
	sf::Vector2f dampingForce = -DAMPING_COEFFICIENT * velocity;

	// Calculate total force (spring + damping)
	sf::Vector2f totalForce = dampingForce + springForce;
	// Applying total camera force (scaled by deltaTime) onto old Position
	sf::Vector2f newCamPos = camPos + (totalForce * deltaTime);

	setViewPos(newCamPos);
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
		return player;
	}

	return makePlayer();
}

bool GI_Arena::handleEvent(sf::Event* eventRef)
{
	if (activeMenu)
		return activeMenu->handleEvent(eventRef);
	return false;
}