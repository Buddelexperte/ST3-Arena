#include "Button.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#include <filesystem> 

Button::Button(const sf::Vector2f& pos, const sf::Vector2f& b_size, const sf::Color& b_color, const unsigned int& t_size, const std::string& t_text, const sf::Color& t_color)
{
	if (!font.loadFromFile("../Content/fonts/coolvetica/coolvetica_rg.otf"))
	{
		std::cout << "Unable to load font!!" << std::endl;
	}

	B_Box.setSize(b_size);
	B_Box.setPosition(pos);
	B_Box.setOrigin(b_size.x / 2.0f, b_size.y / 2.0f);
	B_Box.setFillColor(b_color);

	T_Text.setFont(font);
	T_Text.setString(t_text);
	T_Text.setCharacterSize(t_size);
	T_Text.setFillColor(t_color);

	T_Text.setOrigin(T_Text.getGlobalBounds().width / 2.0f, T_Text.getGlobalBounds().height / 2.0f);
	T_Text.setPosition({pos.x, pos.y - 6});
}

bool Button::isClicked(const sf::Vector2f& mousePos) const {
	return B_Box.getGlobalBounds().contains(mousePos);
}
bool Button::isHovered(const sf::Vector2f& mousePos) const {
	return B_Box.getGlobalBounds().contains(mousePos);
}