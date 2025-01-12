#pragma once
// SFML Libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
// Own libraries
#include "SoundManager.h"
#include "FontManager.h"

class GI_Arena;

struct ButtonConstruct
{
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Color color;
	unsigned int textSize;
	std::string text;
	sf::Color textColor;
};

class Button : public sf::Drawable
{
private:
	GI_Arena* gameInstance;
	SoundManager& sm = SoundManager::getInstance();
	FontManager& fm = FontManager::getInstance();

	int fontID = 0; // [0] in FontManager.h
	sf::Vector2f lastScreenCenter = { 0.0f, 0.0f };
	
	std::string text;
	ButtonConstruct usedConstr{};
public:
	sf::RectangleShape B_Box;
	sf::Text T_Text;
	// Constructors

	Button();
	Button(const ButtonConstruct& constr);

	void construct(const ButtonConstruct&);

	void setText(const std::string&); // Set the texts content
	std::string getText() const { return text; }
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
	sf::Vector2f getSize() const { return usedConstr.size; }

	bool isMouseOver(const bool& = false); // Check if mouse is over button
	void onClick();
	void onHover();
	void onUnhover();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};