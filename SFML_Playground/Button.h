#pragma once
// Own libraries
#include "SoundManager.h"
#include "FontManager.h"
#include "DrawableShape.h"

class GI_Arena;

struct RawButton
{
	sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f size = sf::Vector2f(100.0f, 100.0f);
	sf::Color color = sf::Color::White;
	unsigned int textSize = 0;
	std::string text = "Button Text";
	sf::Color textColor = sf::Color::White;
};

class Button : public IDrawableShapes
{
private:
	FontManager::Font font = FontManager::Font::BUTTON_FONT; // [0] in FontManager.h
	
	RawButton buttonData;
public:
	sf::RectangleShape B_Box;
	sf::Text T_Text;
	// Constructors

	Button();
	Button(const RawButton& constr);

	void construct(const RawButton&);

	void setText(const std::string&); // Set the texts content
	std::string getText() const { return buttonData.text; }
	void setColor(const sf::Color&, const bool& = false); // Set the color of the text or the button fill
	sf::Color getColor(const bool& = false) const; // Get the color of the text or the button fill

	void setTexture(const sf::Texture&, const bool);
	void clearTexture() { return B_Box.setTexture(nullptr); };
	sf::Texture getTexture() const { return *B_Box.getTexture(); }

	void setPos(const sf::Vector2f&); // Move the whole button to a new position
	void addPos(const sf::Vector2f&);
	sf::Vector2f getPos() const { return B_Box.getPosition(); }
	void setRot(const float& newRot) { B_Box.setRotation(newRot); }
	float getRot() const { return B_Box.getRotation(); }
	sf::Vector2f getSize() const { return buttonData.size; }

	bool isMouseOver(const bool& = false); // Check if mouse is over button
	void onClick() const;
	void onHover();
	void onUnhover();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};