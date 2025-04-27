#pragma once
#include <SFML/Graphics.hpp>

#include "WidgetBase.h"

class Slider
{
private:
	sf::RectangleShape bar;
	sf::CircleShape knob;
	float minValue;
	float maxValue;
	float currentValue;
	bool isDragging;

public:
	Slider(float x, float y, float widht, float hight, float min, float max);


};

