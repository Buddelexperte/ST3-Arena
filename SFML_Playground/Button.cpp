#pragma once 
#include "SFML_Arena.h"
#include "Button.h" // Own header file
#include <SFML/Graphics.hpp>
#include <filesystem> // Finding font file
#include <iostream> // Error messages

void Button::construct(const ButtonConstruct& constr)
{
	const sf::Vector2f& pos = constr.pos;
	const sf::Vector2f& b_size = constr.size;
	const sf::Color& b_color = constr.color;
	const unsigned int& t_size = constr.textSize;
	const std::string& t_text = constr.text;
	text = t_text;
	const sf::Color& t_color = constr.textColor;

	// Load text font from project directory
	if (!font.loadFromFile("../Content/fonts/coolvetica/coolvetica_rg.otf"))
	{
		std::cerr << "Unable to load font!!" << std::endl; // Error message for console log
	}
	// Load Click Sound from project directory
	if (!clickBuffer.loadFromFile("..Content/Sounds/Click_Low.wav"))
	{
		std::cout << "Fehler beim Laden vom Click Sound!" << std::endl;
	}

	// Initialize button
	B_Box.setSize(b_size);
	B_Box.setPosition(pos);
	B_Box.setOrigin(b_size.x / 2.0f, b_size.y / 2.0f);
	B_Box.setFillColor(b_color);
	// Initialize Text
	T_Text.setFont(font);
	T_Text.setString(text);
	T_Text.setCharacterSize(t_size);
	T_Text.setFillColor(t_color);
	// Center Text inside Box
	T_Text.setOrigin(T_Text.getGlobalBounds().width / 2.0f, T_Text.getGlobalBounds().height / 2.0f);
	float textYFix = T_Text.getCharacterSize() / 2.6f;
	T_Text.setPosition({ pos.x, pos.y - textYFix});
}

void Button::onClick()
{
	clickSound.setBuffer(clickBuffer);
	clickSound.play();
}

bool Button::isMouseOver(const bool& registerClick) {
	GI_Arena& gameInstance = GI_Arena::getInstance();
	sf::RenderWindow* window = gameInstance.getWindow();
	sf::Vector2f worldMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	if (registerClick) onClick();
	return B_Box.getGlobalBounds().contains(worldMousePos); // Check if the button contains the mouse
}

void Button::setText(const std::string& text)
{
	T_Text.setString(text);
	setPos(B_Box.getPosition()); // Recenter Text (idk why I need that but it works)
}

void Button::setColor(const sf::Color& color, const bool& bTextColor)
{
	(bTextColor ? T_Text.setFillColor(color) : B_Box.setFillColor(color)); // set either box or text fillcolor
}

sf::Color Button::getColor(const bool& bTextColor) const
{
	return (bTextColor ? T_Text.getFillColor() : B_Box.getFillColor()); // return either box or text fillcolor
}

void Button::setTexture(const sf::Texture& newText, const bool resetTint = false)
{
	B_Box.setTexture(&newText);
	if (resetTint) B_Box.setFillColor(sf::Color::White);
}

void Button::setPos(const sf::Vector2f& newPos = {0.0f, 0.0f})
{
	// Update Box position and origin
	B_Box.setPosition(newPos);
	B_Box.setOrigin(B_Box.getSize().x / 2.0f, B_Box.getSize().y / 2.0f);
	// Update Text position and origin
	T_Text.setOrigin(T_Text.getGlobalBounds().width / 2.0f, T_Text.getGlobalBounds().height / 2.0f);
	T_Text.setPosition({ newPos.x, newPos.y - 6.0f });
}

void Button::addPos(const sf::Vector2f& x)
{
	B_Box.setPosition(B_Box.getPosition() + x);
	T_Text.setPosition(T_Text.getPosition() + x);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(B_Box, states); // Draw box
	target.draw(T_Text, states); // Draw text
}