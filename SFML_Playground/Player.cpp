#pragma once
#include "SFML_Arena.h"

Player::Player(WidgetElement* parent = nullptr) : InputWidget(parent)
{
	windowUpdate();
	ButtonConstruct playerButtonConstr = {
		windowCenter + sf::Vector2f{ 0, 0 }, sf::Vector2f(100.0f, 100.0f), sf::Color::Red, 12, "P1", sf::Color::Black
	};
	playerModel = new Button(playerButtonConstr);
	shapes = { playerModel };
}

void Player::update(const float& deltaTime)
{
	windowUpdate();
	// Only check for movement input when GameInstance is not paused
	if (!gameInstance.getIsPaused()) calcMovement();
	// Get user event and distribute events unhandled by player.cpp
	sf::Event event;
	while (window->pollEvent(event) && gameInstance.getGameState() != QUIT)
	{
		// If close event got called, close window and break input wait loop
		if (event.type == sf::Event::Closed)
		{
			window->close();
			break;
		}
		// Event Handler
		handleInput(&event);
	}
	// Draw new Menu to screen through GameInstance
	gameInstance.updateScreen();
}

void Player::calcMovement()
{
	float x = 0.0f;
	float y = 0.0f;

	// Check if each key is currently pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) y -= 1.0f; // Move North
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) x += 1.0f; // Move East
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) y += 1.0f; // Move South
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) x -= 1.0f; // Move West
	
	float newRot = getLookAtRot(getPos(), static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
	setRot(newRot);
	playerModel->move(playerModel->getPos() + (sf::Vector2f(x, y)));
}

sf::Keyboard::Key Player::keyboardInput(sf::Event* eventRef)
{
	sf::Keyboard::Key inputKey = eventRef->key.code;
	switch (inputKey)
	{
	case sf::Keyboard::Escape:
		gameInstance.handleEvent(eventRef);
		break;
	case NORTH: case EAST: case SOUTH: case WEST:
		break;
	default:
		break;
	}
	return eventRef->key.code;
}

sf::Mouse::Button Player::mouseInput(sf::Event* eventRef)
{
	sf::Mouse::Button mouseInput = eventRef->mouseButton.button;
	const bool checkForClick = false;
	switch (mouseInput)
	{
	case sf::Mouse::Left:
		gameInstance.handleEvent(eventRef);
		break;
	default:
		break;
	}
	return mouseInput;
}

// Get und Set Attribute

void Player::setRot(const float& newRot)
{
	playerModel->setRot(newRot);
}

float Player::getRot()
{
	return playerModel->getRot();
}

sf::Vector2f Player::getPos()
{
	return playerModel->getPos();
}