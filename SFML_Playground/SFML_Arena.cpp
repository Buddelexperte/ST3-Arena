#pragma once 
#include "SFML_Arena.h"


// Game Instance Code

GI_Arena::GI_Arena()
{
	window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "SFML_Arena", sf::Style::Fullscreen);
	states = sf::RenderStates::Default;
}

void GI_Arena::updateScreen()
{
	// Clear viewport for new draw
	window->clear(sf::Color::Black);
	// Draw all Drawables from shapes vector
	if (activeWidget != nullptr) window->draw(*activeWidget);
	// Display Draw changes
	window->display();
}

void GI_Arena::setGameState(const E_GameState& newGS)
{
	gameState = newGS;
}


// WidgetMenu Code --------------------------------------------------------------------------------

void WidgetElement::windowUpdate()
{
	window = gameInstance.getWindow();
	windowSize = window->getSize();
	windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
}

void WidgetElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& elem : shapes) target.draw(*elem, states);
}


// InputWidget ------------------------------------------------------------------------------------

bool InputWidget::handleInput(sf::Event* eventRef)
{
	event = eventRef;
	switch (event->type)
	{
	case sf::Event::KeyPressed:
		//std::cout << "[Keyboard Event]" << std::endl;
		return keyboardInput(event);
	case sf::Event::MouseButtonPressed:
		//std::cout << "[Mouse Event]" << std::endl;
		return mouseInput(event);
	case sf::Event::MouseWheelScrolled:
		//std::cout << "[Mouse Wheel Event]" << std::endl;
		return scrollInput(event);
	default:
		break;
	}

	return false;
}

sf::Keyboard::Key InputWidget::keyboardInput(sf::Event* eventRef)
{
	switch (eventRef->key.code)
	{
	case sf::Keyboard::Escape:
		gameInstance.getActiveWidget()->input_esc();
		break;
	default:
		break;
	}
	return eventRef->key.code;
}

sf::Mouse::Button InputWidget::mouseInput(sf::Event* eventRef)
{
	sf::Mouse::Button mouseInput = eventRef->mouseButton.button;
	const bool checkForClick = false;
	if (isMouseOver(checkForClick))
	{
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
	return sf::Mouse::ButtonCount;
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