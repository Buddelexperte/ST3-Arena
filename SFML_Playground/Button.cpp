#include "Button.h"
#include <SFML/Graphics.hpp>

Button::Button()
{

}

Button::Button(const sf::Vector2f& size, const float& xPos, const float& yPos, const std::string& text, const sf::Color& color)
{
	setSize(size);
	setPos(xPos, yPos);
	setText(text);
	setColor(color);
}
bool Button::setPos(const float& NewX, const float& NewY)
{
	B_Box.setPosition(NewX, NewY);
	return true;
}
bool Button::setSize(const sf::Vector2f& newSize)
{
	B_Box.setSize(newSize);
	return true;
}
bool Button::setText(const std::string& newT)
{
	T_Text.setString(newT);
	return true;
}
bool Button::setColor(const sf::Color& newC)
{
	B_Box.setFillColor(newC);
	return true;
}