#pragma once
#include <SFML/Graphics.hpp>

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
	sf::Vector2f lastScreenCenter = sf::Vector2f(0.0f, 0.0f);
public:
	// shapes
	sf::RectangleShape B_Box;
	sf::Text T_Text;
	// Constructors
	Button() : Button({ 0, 0 }, { 0, 0 }, sf::Color::White, 24, "Text", sf::Color::Black) {};
	Button(const ButtonConstruct& constr) : Button(constr.pos, constr.size, constr.color, constr.fontSize, constr.text, constr.textColor) {};
	Button(const sf::Vector2f&, const sf::Vector2f&, const sf::Color&, const unsigned int&, const std::string&, const sf::Color&);

	void setText(const std::string&); // Set the texts content
	void setColor(const sf::Color&, const bool& = false); // Set the color of the text or the button fill
	sf::Color getColor(const bool& = false) const; // Get the color of the text or the button fill

	void setPos(const sf::Vector2f&); // Move the whole button to a new position
	void addPos(const sf::Vector2f&);
	sf::Vector2f getPos() const { return B_Box.getPosition(); }
	void setRot(const float& newRot) { B_Box.setRotation(newRot); }
	float getRot() const { return B_Box.getRotation(); };

	bool isMouseOver() const; // Check if mouse is over button
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};