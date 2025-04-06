#include "Slider.h"
#include <SFML/Graphics.hpp>

Slider::Slider(float x, float y, float widht, float hight, float min, float max)
{
	minValue = min;
	maxValue = max;
	currentValue = min;
	isDragging = false;

	bar.setSize(sf::Vector2f(widht, hight));
	bar.setFillColor(sf::Color(255, 255, 255));
	bar.setPosition(x, y);

	knob.setRadius(hight / 2);
	knob.setFillColor(sf::Color(0, 0, 0));
	knob.setPosition(x - hight / 2, y);
}