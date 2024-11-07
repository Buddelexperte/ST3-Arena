#pragma once
#include "SFML_Arena.h"

class Player : public InputWidget
{
private:
	Button* playerModel = nullptr;
public:
	Player(WidgetElement* parent = nullptr) : InputWidget(parent)
	{
		windowUpdate();
		ButtonConstruct playerButtonConstr = {
			windowCenter + sf::Vector2f{ 0, 0 }, sf::Vector2f(100.0f, 100.0f), sf::Color::Red, 12, "P1", sf::Color::Black
		};
		playerModel = new Button(playerButtonConstr);
		shapes = { playerModel };
	}
};