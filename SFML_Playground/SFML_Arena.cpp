#pragma once 
#include "SFML_Arena.h"


// Game Instance Code
GI_Arena::GI_Arena()
{
	window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "SFML_Clicker", sf::Style::Fullscreen);
	states = sf::RenderStates::Default;
	update();
}

void GI_Arena::update()
{
	// Update viewport values
	windowSize = window->getSize();
	windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
}

void GI_Arena::draw(sf::Drawable* drawable)
{
	// Clear viewport for new draw
	window->clear(sf::Color::Black);
	// Draw all Drawables from shapes vector
	if (drawable != nullptr) window->draw(*drawable);
	// Display Draw changes
	window->display();
}

void GI_Arena::setGameState(const E_GameState& newGS)
{
	gameState = newGS;
}

bool GI_Arena::getGameStateChanges(E_GameState& oldGS) const
{
	if (oldGS != gameState)
	{
		oldGS = gameState;
		return true;
	}
	return false;
}


// WidgetMenu Code
void WidgetMenu::windowUpdate()
{
	window = gameInstance.getWindow();
	windowSize = window->getSize();
	windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
}

void WidgetMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& elem : shapes) target.draw(*elem, states);
}

// InputWidget

bool InputWidget::handleEvent(sf::Event* eventRef)
{
	event = eventRef;
	switch (event->type)
	{
	case sf::Event::KeyPressed:
		return keyboardInput(event);
	case sf::Event::MouseButtonPressed:
		return mouseInput(event);
	case sf::Event::MouseWheelScrolled:
		return scrollInput(event);
	default:
		break;
	}

	return false;
}

sf::Keyboard::Key InputWidget::keyboardInput(sf::Event* eventRef)
{
	return eventRef->key.code;
}

sf::Mouse::Button InputWidget::mouseInput(sf::Event* eventRef)
{
	sf::Mouse::Button mouseInput = eventRef->mouseButton.button;
	if (isMouseOver())
	{
		switch (mouseInput)
		{
		case sf::Mouse::Left:
			if (onMouseClickL()) return sf::Mouse::Left;
			break;
		case sf::Mouse::Right:
			if (onMouseClickR()) return sf::Mouse::Right;
			break;
		case sf::Mouse::Middle:
			if (onMouseClickR()) return sf::Mouse::Middle;
			break;
		default:
			break;
		}
	}
	return sf::Mouse::ButtonCount;
}

float InputWidget::scrollInput(sf::Event* eventRef)
{
	if (event->mouseWheelScroll.wheel != sf::Mouse::VerticalWheel) return 0.0f;
	return eventRef->mouseWheelScroll.delta;
}


// SaveGame Code
int SaveGame::loadSavedData(const std::string& path)
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
		std::cout << "SaveData saved!\n";
		std::cout << SaveGame::Stored_Save << std::endl;
	}
	else {
		std::cerr << "Error opening save file for writing.\n"; // Display file access error message
	}
}