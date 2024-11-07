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

void Player::tick(const float& deltaTime)
{
	windowUpdate();
	// Only check for events if the game started correctly and didn't (technically) end
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

sf::Keyboard::Key Player::keyboardInput(sf::Event* eventRef)
{
	switch (eventRef->key.code)
	{
	case sf::Keyboard::Escape:
		gameInstance.handleEvent(eventRef);
		break;
	case sf::Keyboard::W: case sf::Keyboard::A: case sf::Keyboard::S: case sf::Keyboard::D:
		std::cout << "MOVING!!!" << std::endl;
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