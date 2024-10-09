#include <SFML/Graphics.hpp>
#pragma once

struct ButtonConstruct
{
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Color color;
	unsigned int fontSize;
	std::string text;
	sf::Color textColor;
};

class Button : public sf::Drawable
{
private:
	sf::Font font; // Text font
public:
	// shapes
	sf::RectangleShape B_Box;
	sf::Text T_Text;
	// Constructors
	Button() : Button({ 0, 0 }, { 0, 0 }, sf::Color::White, 24, "Text", sf::Color::Black) {};
	Button(const ButtonConstruct& constr) : Button(constr.pos, constr.size, constr.color, constr.fontSize, constr.text, constr.textColor) {};
	Button(const sf::Vector2f&, const sf::Vector2f&, const sf::Color&, const unsigned int&, const std::string&, const sf::Color&);

	void move(const sf::Vector2f&); // Move the whole button to a new position
	bool isMouseOver(const sf::Vector2f& mousePos) const; // Check if mouse is over button

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

