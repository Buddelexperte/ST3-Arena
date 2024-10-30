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
	window->draw(*drawable);
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
	windowSize = window->getSize();
	windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
}

void WidgetMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& elem : shapes) target.draw(*elem, states);
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