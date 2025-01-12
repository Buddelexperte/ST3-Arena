#pragma once 
#include <fstream>
#include "BaseClasses.h"
#include "GameInstance.h"
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
	prevCamPos = view->getCenter();
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
		widgets.push_back(std::make_shared<W_MainMenu>(nullptr)); // MainMenu = 0
		widgets.push_back(std::make_shared<W_Gameplay>(nullptr)); // Gameplay = 1
		std::cout << "Initiated widgets" << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		widgets.clear(); // Cleanup in case of partial initialization
		return false;
	}
}

bool GI_Arena::makePlayer()
{
	if (player) return false;
	player = new Player(nullptr);
	std::cout << "Player created" << std::endl;
	return true;
}

void GI_Arena::start()
{
	fontManager.loadFonts(); // No lazy loading for fonts
	makePlayer();
	
	initWidgets();

	correctWidget();

	std::cout << "\n### Starting Game ###\n" << std::endl;

	// Main Game Loop
	while (window->isOpen())
	{
		preTick();
		// Calculate deltaTime for time corrected physics
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
	prevCamPos = view->getCenter();
	soundManager.cleanUp();
}

void GI_Arena::tick(const float& deltaTime)
{
	tickView(deltaTime);

	player->update(deltaTime);
	activeMenu->update(deltaTime);
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

// ReCenter sf::View position on current Player position and set prevCamPos
void GI_Arena::resetViewPos()
{
	setViewPos(getPlayer()->getPos());
	prevCamPos = view->getCenter();
}

// Adjust the camera position based on velocity and distance to player (Spring-Damp-System)
void GI_Arena::tickView(const float& deltaTime)
{
	constexpr float SPRING_STRENGTH = 4.0f; // Higher = Quicker follow
	constexpr float DAMPING_COEFFICIENT = 1.0f; // Higher = More Resistance
	constexpr float MAX_DISTANCE = 150.0f; // Max allowed distance in x and y

	// Get current camera and player positions
	const sf::Vector2f& camPos = view->getCenter();
	const sf::Vector2f& playerPos = getPlayer()->getPos();

	// Calculate distance between both positions (x and y)
	sf::Vector2f distance = camPos - playerPos;

	// Apply MAX_DISTANCE constraint to avoid camera clipping player
	if (std::abs(distance.x) > MAX_DISTANCE)
	{
		distance.x = (distance.x > 0 ? MAX_DISTANCE : -MAX_DISTANCE);
	}
	if (std::abs(distance.y) > MAX_DISTANCE)
	{
		distance.y = (distance.y > 0 ? MAX_DISTANCE : -MAX_DISTANCE);
	}

	// Calculate Spring Force (Using distance to target)
	sf::Vector2f springForce = -SPRING_STRENGTH * distance;

	// Calculate velocity (Using old and new position)
	sf::Vector2f velocity = camPos - prevCamPos;
	// Calculate Damping Force (Using velocity)
	sf::Vector2f dampingForce = -DAMPING_COEFFICIENT * velocity;

	// Calculate total force (spring + damping)
	sf::Vector2f totalForce = dampingForce + springForce;

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

void GI_Arena::setGameState(const E_GameState& newGS)
{
	gameState = newGS;
}

Player* GI_Arena::getPlayer()
{
	return player;
}

bool GI_Arena::handleEvent(sf::Event* eventRef)
{
	return activeMenu->handleInput(eventRef);
}


// WidgetMenu Code --------------------------------------------------------------------------------

WidgetElement::WidgetElement(InputWidget* parentWidget)
	: parent(parentWidget), gameInstance(&GI_Arena::getInstance()), window(gameInstance->getWindow())
{
	ORIGIN = window->getView().getCenter();
	windowUpdate();
}

void WidgetElement::windowUpdate()
{
	// Everything sf::RenderWindow related
	window = gameInstance->getWindow();
	windowSize = window->getSize();
	windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
	// Everything sf::View related
	view = gameInstance->getView();
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
		lockWeakPtr( gameInstance->getActiveWidget() )->input_esc();
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