#pragma once
#include <iostream>

#include "GameInstance.h"
#include "Widgets.h"
#include "EntityManager.h"
#include "SoundManager.h"
#include "Player.h"

#include <filesystem>

float GI_Arena::globalTime = 0.0f;

GI_Arena::GI_Arena()
{
	usedSettings = UserSettings::loadSettings();

	// Loading taskbar window icon
	if (!window_icon.loadFromFile("Content/Arena_icon.png"))
	{
		std::cerr << "Could not load window_icon from path!" << std::endl;
	}

	setMaxFPS(usedSettings.maxFPS);
	
	setUseVSync(usedSettings.bUseVSync);

	setUseWidgetParallax(usedSettings.bWidgetParallax);

	createViewport();
	
	std::cout << "Initiated viewport\n" << std::endl;

}

void GI_Arena::createViewport()
{
	std::cout << "### Creating viewport..." << std::endl;

	sf::VideoMode mode = UserSettings::getResolution(usedSettings.resID);

	if (window)
	{
		window->close();
		window.release();
	}

	const sf::Uint32 style = (usedSettings.bFullscreen && !getIsDebugMode() ? sf::Style::Fullscreen : sf::Style::Default);
	window = std::make_unique<sf::RenderWindow>(mode, WINDOW_NAME, style);
	std::cout << "- RenderWindow created" << std::endl;

	// Applying settings
	if (usedSettings.maxFPS > 0)
		window->setFramerateLimit(usedSettings.maxFPS);
	window->setVerticalSyncEnabled(usedSettings.bUseVSync);

	// If window_icon was loaded, apply to window
	if (window_icon.getPixelsPtr())
	{
		window->setIcon(window_icon.getSize().x, window_icon.getSize().y, window_icon.getPixelsPtr());
		std::cout << "- Applied window icon" << std::endl;
	}
	else
	{
		std::cout << "- No valid window icon to apply" << std::endl;
	}

	// If view is not valid, create one
	if (!view)
	{
		const sf::Vector2f size{ static_cast<float>(mode.width), static_cast<float>(mode.height) };
		view = std::make_unique<sf::View>(size / 2.f, size);
		std::cout << "- View created" << std::endl;
	}

	// Apply view and set cache values
	prevCamPos = widgetOffset = view->getCenter();
	window->setView(*view);
	std::cout << "- View attached" << std::endl;
}

void GI_Arena::setViewportValues(size_t resolutionID, bool bFullscreen)
{
	bool noResDiff = (resolutionID == usedSettings.resID);
	bool noFullscreenDiff = (bFullscreen == usedSettings.bFullscreen);

	if (noResDiff && noFullscreenDiff)
		return;

	sf::VideoMode mode = UserSettings::getResolution(usedSettings.resID);

	if (bFullscreen)
	{
		bool valid = false;
		for (const auto& m : sf::VideoMode::getFullscreenModes())
		{
			if (m.width == mode.width && m.height == mode.height)
			{
				valid = true;
				break;
			}
		}

		if (!valid)
		{
			std::cerr << "Resolution " << mode.width << "x" << mode.height << " not supported in fullscreen.\n";
			resolutionID = 0;
			bFullscreen = true;
		}
	}

	usedSettings.resID = resolutionID;
	usedSettings.bFullscreen = bFullscreen;

	createViewport();
}

bool GI_Arena::initWidgets()
{
	// Updating or initially setting DebugIndicator
	T_DebugMode.construct();

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

Player* GI_Arena::getValidPlayer()
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

	getValidPlayer();

	initWidgets();
	
	std::cout << "\n### Starting Game ###\n" << std::endl;

	tickLoop();
}

void GI_Arena::tickLoop()
{

	// Main Game Loop
	while (window->isOpen())
	{
		// Calculate deltaTime for time corrected physics
		const float deltaTime = clock.restart().asSeconds();
		preTick();
		tick(deltaTime);
		postTick();
	}
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

	// Tick components
	T_DebugMode.tick(deltaTime);

	// Event management
	sf::Event event;
	while (window->pollEvent(event) && gameState > QUIT)
	{
		switch (event.type)
		{
		case sf::Event::GainedFocus:
			// Check if developer Mode was configured and display it
			T_DebugMode.construct();
			break;
		case sf::Event::LostFocus:
			// DONT HANDLE TWICE, DO NOT IMPLEMENT PAUSING LOGIC FOR FOCUS LOST IN PLAYER
			activeMenu->handleEvent(&event);
			break;
		case sf::Event::Closed:
			// End tickLoop, return
			return window->close(); 
		default:
			break;
		}

		// Player gets to distribute the events and inputs
		player->handleEvent(&event);
	}

	// Update activeWidget if changes to gameState have been made in last tick()
	correctWidget();

	// Tick activeWidget environment
	if (activeMenu != nullptr)
	{
		activeMenu->tick(deltaTime);
	}
}

void GI_Arena::postTick()
{
	// Check if a sf::Event::Closed was called
	if (window->isOpen())
		// Draw new Menu to screen through GameInstance
		updateScreen();
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
	// Resetting gameplay aspects
	EntityManager::getInstance().deleteAll();
	player->spawn();
	resetViewPos();
}

void GI_Arena::startRound()
{
	// Only activate Gameplay phase when at least launch phase
	if (gameState != GAME_LAUNCHING)
		return;

	// Set gameState to IN_GAME for startRound to work (safety mechanism)
	setGameState(IN_GAME);
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
	constexpr float DAMPING_COEFFICIENT = 4.0f;         // Higher = More Resistance
	constexpr float MAX_DISTANCE = 100.0f;              // Maximum allowed mouse influence
	constexpr float MIN_DELTA = 0.001f;                 // Minimum delta time to prevent extreme forces

	// Clamp deltaTime to prevent extreme forces on first frame or lag spikes
	float clampedDelta = std::max(deltaTime, MIN_DELTA);
	if (clampedDelta > 0.1f) clampedDelta = 0.1f;       // Also prevent too large steps

	bool bIsGameOver = getGameState() == GAME_OVER;

	// Get current camera, player, and mouse positions
	const sf::Vector2f camPos = view->getCenter();
	const sf::Vector2f playerPos = getPlayer()->getPosition();
	const sf::Vector2f mousePos = (bIsGameOver ? playerPos : getMousePos());
	static sf::Vector2f mouseOffset;

	// Calculate the mouse offset relative to the player
	if (usedSettings.bWidgetParallax || !getIsPaused() || bIsGameOver)
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

	// Update widget offset in a separate method
	updateWidgetOffset(mouseOffset, deltaTime);

	IDrawableShapes::updateValues();
}

// New separate method for widget offset calculation
void GI_Arena::updateWidgetOffset(const sf::Vector2f& gameplayMouseOffset, const float& deltaTime)
{
	constexpr float WIDGET_LERP_ALPHA = 0.1f;       // Standard gameplay lerp speed
	constexpr float MENU_LERP_ALPHA = 0.02f;        // Slower for menus (smoother)
	constexpr float MENU_MAX_OFFSET = 650.0f;		// Max menu parallax distance
	constexpr float MENU_DEADZONE = 350.0f;         // Center area with reduced movement
	constexpr float PARALLAX_SCALE = 0.1f;          // Scale factor for parallax effect

	const sf::Vector2f viewCenter = view->getCenter();
	const bool isGameplayActive = getGameState() > MENU_SCREEN;

	if (usedSettings.bWidgetParallax)
	{
		if (isGameplayActive)
		{
			// Use gameplay offset calculation (player-relative)
			const sf::Vector2f newCenter = viewCenter - (gameplayMouseOffset * PARALLAX_SCALE);
			float factor = lerpFactor(deltaTime, WIDGET_LERP_ALPHA);
			widgetOffset = lerp(widgetOffset, newCenter, factor);
		}
		else
		{
			// Menu mode: calculate offset based on mouse position relative to screen center
			sf::Vector2f mousePos = getMousePos();
			sf::Vector2f menuOffset = mousePos - viewCenter;

			// Apply non-linear scaling to reduce sensitivity near center (deadzone)
			float offsetLength = std::sqrt(menuOffset.x * menuOffset.x + menuOffset.y * menuOffset.y);

			if (offsetLength > 0.0f)
			{
				// Apply deadzone and smooth acceleration curve
				float effectiveDistance = std::max(0.0f, offsetLength - MENU_DEADZONE);
				float smoothFactor = std::min(effectiveDistance / (MENU_MAX_OFFSET - MENU_DEADZONE), 1.0f);

				// Apply easing curve to make movement more natural
				smoothFactor = smoothFactor * smoothFactor * (3.0f - 2.0f * smoothFactor); // Smoothstep

				// Scale direction vector by the smooth factor and max distance
				if (offsetLength > MENU_DEADZONE)
				{
					menuOffset = (menuOffset / offsetLength) * MENU_MAX_OFFSET * smoothFactor * PARALLAX_SCALE;
				}
				else
				{
					menuOffset = sf::Vector2f(0.0f, 0.0f);
				}
			}

			// Apply inverse offset to create parallax effect (objects move opposite to mouse)
			const sf::Vector2f newCenter = viewCenter - menuOffset;
			float factor = lerpFactor(deltaTime, MENU_LERP_ALPHA);
			widgetOffset = lerp(widgetOffset, newCenter, factor);
		}
	}
	else
	{
		widgetOffset = viewCenter;
	}
}

void GI_Arena::updateScreen()
{
	// Clear viewport for new draw
	window->clear(sf::Color::Black);
	// Draw all Drawables from shapes vector
	if (activeMenu != nullptr) window->draw(*activeMenu);
	window->draw(T_DebugMode);
	// Display Draw changes
	window->display();
}

// TODO: Rewrite the GameState logic, handling, disconnect from Pausing, reduce control etc
void GI_Arena::setGameState(const GameState& newGS)
{
	// Refuse to acknowledge "change" when nothing changed
	if (newGS == gameState)
		return; // eg when opening Gameplay and animation needs IN_GAME

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

void GI_Arena::applySettings(const UserSettings_Struct settings)
{
	setMaxFPS(settings.maxFPS);
	setUseVSync(settings.bUseVSync);
	setUseWidgetParallax(settings.bWidgetParallax);
	setViewportValues(settings.resID, settings.bFullscreen);

	std::cout << "Applied locally saved settings\n" << std::endl;

	UserSettings::saveSettings(usedSettings);
}

void GI_Arena::setMaxFPS(unsigned int maxFPS)
{
	if (maxFPS == usedSettings.maxFPS)
		return;

	usedSettings.maxFPS = (maxFPS > 0 ? maxFPS : 0);
}

void GI_Arena::setUseVSync(bool bUseVSync)
{
	if (bUseVSync == usedSettings.bUseVSync)
		return;

	usedSettings.bUseVSync = bUseVSync;
}

void GI_Arena::setUseWidgetParallax(bool bWidgetParallax)
{
	if (bWidgetParallax == usedSettings.bWidgetParallax)
		return;

	usedSettings.bWidgetParallax = bWidgetParallax;
}

bool GI_Arena::getIsDebugMode() const
{
	return std::filesystem::exists(devPath);
}

Player* GI_Arena::getPlayer()
{
	if (player)
	{
		return player.get();
	}

	return getValidPlayer();
}

bool GI_Arena::handleEvent(sf::Event* eventRef)
{
	if (activeMenu)
		return activeMenu->handleEvent(eventRef);

	return false;
}