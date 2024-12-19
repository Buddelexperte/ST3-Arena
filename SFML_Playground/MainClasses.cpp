#pragma once 
#include <fstream>
#include "GameInstance.h"
#include "BaseClasses.h"
#include "Widgets.h"

// Game Instance Code

GI_Arena::GI_Arena()
{
	const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	window = new sf::RenderWindow(desktop, "SFML_Arena", sf::Style::Fullscreen);
	
	// Only use for crash heavy debug !
	//window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "SFML_Arena", sf::Style::Titlebar | sf::Style::Default);
	
	std::cout << "RenderWindow created." << std::endl;
	sf::Vector2f desktopSize = { static_cast<float>(desktop.width), static_cast<float>(desktop.height) };
	view = new sf::View(desktopSize / 2.0f, desktopSize);
	std::cout << "View created" << std::endl;
	window->setView(*view);
	std::cout << "View attached" << std::endl;
}

bool GI_Arena::initWidgets()
{
	static bool didOnce = false;

	if (didOnce) return false;
	didOnce = true;

	widgets.clear(); 
	std::cout << "Initiating widgets..." << std::endl;
	try { // Adding base widgets with their nullptr parent HERE!
		widgets.push_back(std::make_shared<W_MainMenu>(nullptr));
		widgets.push_back(std::make_shared<W_Gameplay>(nullptr));
		std::cout << "Initiated widgets" << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		widgets.clear(); // Cleanup in case of partial initialization
		return false;
	}
}

void GI_Arena::start()
{
	fontManager.loadFonts(); // No lazy loading for fonts
	initWidgets();
	correctWidget();

	std::cout << "\n### Starting Game ###\n" << std::endl;

	// Main Game Loop
	while (window->isOpen())
	{
		preTick();
		deltaTime = clock.restart().asSeconds();
		fps = 1.0f / deltaTime;
		tick(deltaTime);
		postTick();
	}
}

void GI_Arena::correctWidget()
{
	// If GameState changed in earlier loop, construct new activeMenu;
	static E_GameState oldGS = QUIT;

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

void GI_Arena::preTick()
{

}

void GI_Arena::tick(const float& deltaTime)
{
	soundManager.cleanUp();

	tickView(deltaTime);

	activeMenu->update(deltaTime);
	playerRef->update(deltaTime);
}

void GI_Arena::postTick()
{
	correctWidget();
	// Draw new Menu to screen through GameInstance
	updateScreen();
}

void GI_Arena::setViewPos(const sf::Vector2f& newPos)
{
	view->setCenter(newPos);
	window->setView(*view);
}

void GI_Arena::tickView(const float& deltaTime) // Updated
{
	// Camera movement settings
	constexpr float CAMERA_SMOOTHNESS = 5.0f; // Higher values = faster catch-up
	constexpr float MAX_DISTANCE = 350.0f; // Maximum allowed distance in x and y directions

	// Get the current camera position and the player's position
	const sf::Vector2f& camPos = view->getCenter();
	const sf::Vector2f& playerPos = getPlayer()->getPos();

	// Calculate the distance vector between the camera and the player
	sf::Vector2f distance = camPos - playerPos;

	// Snap immediately if the distance is below a threshold
	if (shouldZero(distance))
	{
		setViewPos(playerPos);
		return;
	}

	// Target position starts as the current camera position
	sf::Vector2f snappedPos = camPos;

	// Calculate the excess distance in each axis
	sf::Vector2f distanceToMax = { std::abs(distance.x) - MAX_DISTANCE, std::abs(distance.y) - MAX_DISTANCE };

	// Snap to the max distance if the player exceeds the threshold
	if (std::abs(distance.x) > MAX_DISTANCE)
	{
		snappedPos.x = playerPos.x + (distance.x > 0 ? MAX_DISTANCE : -MAX_DISTANCE);
	}

	if (std::abs(distance.y) > MAX_DISTANCE)
	{
		snappedPos.y = playerPos.y + (distance.y > 0 ? MAX_DISTANCE : -MAX_DISTANCE);
	}

	// Determine the target position
	sf::Vector2f targetPos = (snappedPos != camPos && !shouldZero(distanceToMax, 100.0f)
		? snappedPos
		: playerPos);

	// Smoothly follow the player when within bounds using deltaTime
	float lerpFactor = CAMERA_SMOOTHNESS * deltaTime; // Scaled by delta time
	sf::Vector2f newCamPos = lerp(camPos, targetPos, lerpFactor);

	// Update the view position
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

void GI_Arena::setGameState(const E_GameState& newGS)
{
	gameState = newGS;
}

Player* GI_Arena::getPlayer()
{
	if (playerRef == nullptr) playerRef = new Player(nullptr);
	return playerRef;
}

bool GI_Arena::handleEvent(sf::Event* eventRef)
{
	return activeMenu->handleInput(eventRef);
}


// WidgetMenu Code --------------------------------------------------------------------------------

void WidgetElement::windowUpdate()
{
	// Everything sf::RenderWindow related
	window = gameInstance.getWindow();
	windowSize = window->getSize();
	windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
	// Everything sf::View related
	view = gameInstance.getView();
	viewSize = view->getSize();
	viewHalfSize = viewSize / 2.0f;
	viewCenter = view->getCenter();
}

void WidgetElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& elem : shapes) target.draw(*elem, states);
}


// InputWidget ------------------------------------------------------------------------------------

bool InputWidget::handleInput(sf::Event* eventRef)
{
	switch (eventRef->type)
	{
	case sf::Event::KeyPressed:
		//std::cout << "[Keyboard Event]" << std::endl;
		return keyboardInput(eventRef);
	case sf::Event::MouseButtonPressed:
		//std::cout << "[Mouse Event]" << std::endl;
		return mouseInput(eventRef);
	case sf::Event::MouseWheelScrolled:
		//std::cout << "[Mouse Wheel Event]" << std::endl;
		return scrollInput(eventRef);
	default:
		break;
	}

	return false;
}

InputWidget* InputWidget::setWidgetIndex(const int& toIndex)
{
	widgetIndex = toIndex;
	switch (widgetIndex)
	{
	case 0:
		// Implement default shapes in child classes
		break;
	default:
		break;
	}
	return getWidgetAtIndex(toIndex);
}

sf::Keyboard::Key InputWidget::keyboardInput(sf::Event* eventRef)
{
	switch (eventRef->key.code)
	{
	case sf::Keyboard::Escape:
		lockWeakPtr( gameInstance.getActiveWidget() )->input_esc();
		break;
	default:
		break;
	}
	return eventRef->key.code;
}

sf::Mouse::Button InputWidget::mouseInput(sf::Event* eventRef)
{
	const sf::Mouse::Button mouseInput = eventRef->mouseButton.button;

	switch (mouseInput)
	{
	case sf::Mouse::Left:
		onMouseClickL();
		break;
	case sf::Mouse::Right:
		onMouseClickR();
		break;
	case sf::Mouse::Middle:
		onMouseClickR();
		break;
	default:
		break;
	}
	return mouseInput;
}

float InputWidget::scrollInput(sf::Event* eventRef)
{
	if (event->mouseWheelScroll.wheel != sf::Mouse::VerticalWheel) return 0.0f;
	return eventRef->mouseWheelScroll.delta;
}


// SaveGame Code ----------------------------------------------------------------------------------

int SaveGame::Stored_Save = SaveGame::loadSavedData(SAVE_FILE);

int SaveGame::loadSavedData(const std::string& path = SAVE_FILE)
{
	std::ifstream inFile(path);  // Open file in input mode and write the highscore to it
	if (inFile.is_open()) {
		inFile >> SaveGame::Stored_Save;
		inFile.close();
		std::cout << "SaveData loaded!\n";
	}
	else {
		std::cerr << "Error opening save file for reading. Defaulting to 0.\n"; // Display file access error message
	}
	return SaveGame::Stored_Save;
}

void SaveGame::saveData()
{
	std::ofstream outFile(SAVE_FILE); // Open file in output mode and write the highscore to it
	if (outFile.is_open()) {
		outFile << SaveGame::Stored_Save;
		outFile.close();
		std::cout << "SaveData saved! (" << std::to_string(SaveGame::Stored_Save) << ")\n";
	}
	else {
		std::cerr << "Error opening save file for writing.\n"; // Display file access error message
	}
}