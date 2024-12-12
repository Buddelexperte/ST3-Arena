#pragma once 
#include "GameInstance.h"
#include "BaseClasses.h"
#include <fstream>
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
	view = new sf::View(desktopSize / 2.0f, desktopSize); // Arbitrary position
	std::cout << "View created" << std::endl;
	window->setView(*view);
	std::cout << "View attached" << std::endl;
}

bool GI_Arena::initWidgets()
{
	try {
		widgets.clear();
		widgets.push_back(std::make_shared<W_MainMenu>(nullptr));
		widgets.push_back(std::make_shared<W_Gameplay>(nullptr));
		std::cout << "Initiated widgets..." << std::endl;
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
	fontManager.loadFonts();
	initWidgets();

	std::cout << "Starting Game" << std::endl;

	preTick();
	// Main Game Loop
	while (window->isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		fps = 1.0f / deltaTime;
		tick(deltaTime);
		preTick();
	}
}

void GI_Arena::preTick()
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

void GI_Arena::tick(const float& deltaTime)
{
	soundManager.cleanUp();

	sf::Vector2f playerPos = getPlayer()->getPos();
	if (view->getCenter() != playerPos) {
		const float cameraSmoothing = 0.005f;
		sf::Vector2f newCenter = lerp(view->getCenter(), playerPos, cameraSmoothing); // Smooth camera
		setViewCenter(newCenter);
	}

	activeMenu->update(deltaTime);
	playerRef->update(deltaTime);
}

void GI_Arena::setViewCenter(const sf::Vector2f& newCenter)
{
	view->setCenter(newCenter);
	window->setView(*view);
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