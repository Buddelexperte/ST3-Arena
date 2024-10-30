#pragma once 
#include "SFML_Arena.h"

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
	window->clear();
	// Draw all Drawables from shapes vector
	window->draw(*drawable);
	// Display Draw changes
	window->display();
}

void GI_Arena::setGameState(const E_GameState& newGS)
{
	gameState = newGS;
}