#pragma once
#include "SFML_Arena.h"

class Player : public InputWidget
{
private:
	Button* playerModel = nullptr;
public:
	Player() : InputWidget()
	{
		ButtonConstruct playerButtonConstr = {
			windowCenter + sf::Vector2f{ 0, 0 }, sf::Vector2f(200.0, 200.0f), sf::Color::Red, 12, "P1", sf::Color::Black
		};
		playerModel = new Button(playerButtonConstr);
		shapes = { playerModel };
	}
};