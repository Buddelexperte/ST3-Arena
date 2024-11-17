#pragma once
#include "SFML_Arena.h"

Player::Player(WidgetElement* parent = nullptr) : InputWidget(parent)
{
	ButtonConstruct playerButtonConstr = {
		windowCenter + sf::Vector2f{ 0, 0 }, sf::Vector2f(100.0f, 100.0f), sf::Color::Red, 12, "P1", sf::Color::Black
	};
	playerModel = new Button(playerButtonConstr);
	shapes = { playerModel };
}

void Player::update(const float& deltaTime)
{
	InputWidget::update(deltaTime); // Call parent function to set necessary values and call base functions
	if (!gameInstance.getIsPaused()) calcMovement(); // Only check for movement input when GameInstance is not paused
	// Collect / Receive user events
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
		handleInput(&event); // Distinguishes between mouse, keyboard etc.
	}
	// Draw new Menu to screen through GameInstance
	gameInstance.updateScreen();
}

void Player::calcMovement()
{
	float x = 0.0f; // X-Movement per frame
	float y = 0.0f; // Y-Movement per frame
	float multiplier = 1.0f; // Speed multiplier
	// Multiplier for sprinting, damaged etc.
	if (sf::Keyboard::isKeyPressed(KEY_LSHIFT)) multiplier *= 2.0f;
	// Check if each key is currently pressed and modify velocity accordingly
	if (sf::Keyboard::isKeyPressed(KEY_W)) y -= 1.0f; // Move North
	if (sf::Keyboard::isKeyPressed(KEY_A)) x -= 1.0f; // Move West
	if (sf::Keyboard::isKeyPressed(KEY_S)) y += 1.0f; // Move South
	if (sf::Keyboard::isKeyPressed(KEY_D)) x += 1.0f; // Move East
	// Add velocity * multiplier to PlayerPosition
	velocity = { x * multiplier, y * multiplier };
	addPos(velocity);
	// Rotate player sprite to current mouse pos on screen
	sf::Vector2f playerPos = getPos();
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
	float newRot = getLookAtRot(playerPos, mousePos);
	setRot(newRot);
}
// Override class default keyboard Input to check for specific cases
sf::Keyboard::Key Player::keyboardInput(sf::Event* eventRef)
{
	sf::Keyboard::Key inputKey = eventRef->key.code;
	switch (inputKey)
	{
	case sf::Keyboard::Escape: // Esc goes through gameInstance to handle widget input
		gameInstance.handleEvent(eventRef);
		break;
	default:
		break;
	}
	return eventRef->key.code;
}

// Override class default mouse Input to check for specific cases
sf::Mouse::Button Player::mouseInput(sf::Event* eventRef)
{
	sf::Mouse::Button mouseInput = eventRef->mouseButton.button;
	const bool checkForClick = false;
	switch (mouseInput)
	{
	case sf::Mouse::Left: // LMB goes through gameInstance to handle widget inputs
		gameInstance.handleEvent(eventRef);
		break;
	default:
		break;
	}
	return mouseInput;
}

// Get und Set Attribute

void Player::addPos(const sf::Vector2f& x)
{
	playerModel->addPos(x);
}

sf::Vector2f Player::getPos()
{
	return playerModel->getPos();
}

void Player::setRot(const float& newRot)
{
	playerModel->setRot(newRot);
}

float Player::getRot()
{
	return playerModel->getRot();
}