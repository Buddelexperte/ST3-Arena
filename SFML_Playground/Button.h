#include <SFML/Graphics.hpp>
#pragma once

class Button : public sf::Drawable
{
private:
	sf::Font font;

	sf::RectangleShape B_Box;
	sf::Text T_Text;

	sf::Vector2f size;
	float xPos;
	float yPos;
	std::string text;
	sf::Color color;
public:
	Button();
	Button(const sf::Vector2f& size, const float& xPos, const float& yPos, const std::string& text, const sf::Color& color);
	bool setPos(const float& NewX, const float& NewY);
	bool setSize(const sf::Vector2f& newSize);
	bool setText(const std::string& newT);
	bool setColor(const sf::Color& newC);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(B_Box, states);
		target.draw(T_Text, states);
	}
};

