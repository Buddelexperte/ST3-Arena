#pragma once
#include "SFML_Arena.h"

Player::Player(InputWidget* parent) : InputWidget(parent)
{
	ButtonConstruct playerButtonConstr = {
		windowCenter + sf::Vector2f{ 0, 0 }, sf::Vector2f(100.0f, 100.0f), sf::Color::Red, 12, "P1", sf::Color::Black
	};
	playerModel.construct(playerButtonConstr);
	shapes = { &playerModel };
}

void Player::update(const float& deltaTime)
{
	InputWidget::update(deltaTime); // Call parent function to set necessary values and call base functions
	if (!gameInstance.getIsPaused()) calcMovement(deltaTime); // Only check for movement input when GameInstance is not paused
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

void Player::calcMovement(const float& deltaTime)
{
	// Constants
	const float WALKING_SPEED = 350.0f;
	const float LERP_SMOOTHNESS = 0.01f;

	float x = 0.0f; // X-Movement per frame
	float y = 0.0f; // Y-Movement per frame
	float multiplier = 1.0f; // Speed multiplier

	if (sf::Keyboard::isKeyPressed(KEY_LSHIFT)) 
		multiplier *= 2.0f;

	// Check if each key is currently pressed and modify velocity accordingly
	if (sf::Keyboard::isKeyPressed(KEY_W)) y -= WALKING_SPEED; // Move North
	if (sf::Keyboard::isKeyPressed(KEY_A)) x -= WALKING_SPEED; // Move West
	if (sf::Keyboard::isKeyPressed(KEY_S)) y += WALKING_SPEED; // Move South
	if (sf::Keyboard::isKeyPressed(KEY_D)) x += WALKING_SPEED; // Move East
	
	// Target velocity (scaled by deltaTime)
	sf::Vector2f targetVelo = { x * multiplier * deltaTime, y * multiplier * deltaTime };

	// Smoothly interpolate velocity using linear interpolation
	if (velocity != targetVelo)
		velocity = lerp(velocity, targetVelo, LERP_SMOOTHNESS);

	// Update position
	addPos(velocity);

	// Rotation
	sf::Vector2f playerPos = getPos();
	sf::Vector2f mousePos = gameInstance.getMousePos();
	float rotation = getRot();
	float newRot = getLookAtRot(playerPos, mousePos);

	// Smoothly interpolate rotation using lerp
	if (rotation != newRot)
		setRot(lerp(rotation, newRot, LERP_SMOOTHNESS * 10.0f));
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
	case sf::Mouse::Left: case sf::Mouse::Right: // LMB goes through gameInstance to handle widget inputs
		gameInstance.handleEvent(eventRef);
		break;
	default:
		break;
	}
	return mouseInput;
}

float Player::scrollInput(sf::Event* eventRef)
{
	return eventRef->mouseWheelScroll.delta; // TODO - heavy
	float zoomfactor = 1 + (eventRef->mouseWheelScroll.delta * 0.1);
	gameInstance.getView()->zoom(zoomfactor);
	return eventRef->mouseWheelScroll.delta;
}


// Get und Set Attribute

void Player::setPos(const sf::Vector2f& newPos)
{
	playerModel.setPos(newPos);
}

void Player::addPos(const sf::Vector2f& x)
{
	playerModel.addPos(x);
}

sf::Vector2f Player::getPos() const
{
	return playerModel.getPos();
}

void Player::setRot(const float& newRot)
{
	playerModel.setRot(newRot);
}

float Player::getRot() const
{
	return playerModel.getRot();
}