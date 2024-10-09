#include "Button.h" // Own header file
#include <SFML/Graphics.hpp>
#include <filesystem> // Finding font file
#include <iostream> // Error messages
#pragma once 

Button::Button(const sf::Vector2f& pos, const sf::Vector2f& b_size, const sf::Color& b_color, const unsigned int& t_size, const std::string& t_text, const sf::Color& t_color)
{
	// Load text font from project directory
	if (!font.loadFromFile("../Content/fonts/coolvetica/coolvetica_rg.otf"))
	{
		std::cout << "Unable to load font!!" << std::endl; // Error message for console log
	}
	// Initialize button
	B_Box.setSize(b_size);
	B_Box.setPosition(pos);
	B_Box.setOrigin(b_size.x / 2.0f, b_size.y / 2.0f);
	B_Box.setFillColor(b_color);
	// Initialize Text
	T_Text.setFont(font);
	T_Text.setString(t_text);
	T_Text.setCharacterSize(t_size);
	T_Text.setFillColor(t_color);
	// Center Text inside Box
	T_Text.setOrigin(T_Text.getGlobalBounds().width / 2.0f, T_Text.getGlobalBounds().height / 2.0f);
	T_Text.setPosition({pos.x, pos.y - 6.0f});
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
	return B_Box.getGlobalBounds().contains(mousePos); // Check if Box bounds contain position of mouse
}

void Button::setText(const std::string& text)
{
	T_Text.setString(text);
	move(B_Box.getPosition());
}

void Button::move(const sf::Vector2f& newPos = {0.0f, 0.0f})
{
	// Update Box position and origin
	B_Box.setPosition(newPos);
	B_Box.setOrigin(B_Box.getSize().x / 2.0f, B_Box.getSize().y / 2.0f);
	// Update Text position and origin
	T_Text.setOrigin(T_Text.getGlobalBounds().width / 2.0f, T_Text.getGlobalBounds().height / 2.0f);
	T_Text.setPosition({ newPos.x, newPos.y - 6.0f });
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(B_Box, states); // Draw box
	target.draw(T_Text, states); // Draw text
}