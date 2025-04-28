#pragma once
// Own libraries
#include "FontManager.h"
#include "WidgetBase.h"

enum EAlignment
{
	LEFT_BOTTOM,
	LEFT,
	LEFT_TOP,
	CENTER_BOTTOM,
	CENTER,
	CENTER_TOP,
	RIGHT_BOTTOM,
	RIGHT,
	RIGHT_TOP
};

// Pos, Size, Color, TextSize, TextString, TextColor, Alignment, TextAlignment
struct RawButton
{
	sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f size = sf::Vector2f(100.0f, 100.0f);
	sf::Color color = sf::Color::White;
	unsigned int textSize = 0;
	std::string text = "Button Text";
	sf::Color textColor = sf::Color::White;
	EAlignment alignment = EAlignment::CENTER;
	EAlignment textAlignment = EAlignment::CENTER;
};

class Button : public WidgetElement
{
private:
	FontManager::Font font = FontManager::Font::BUTTON_FONT; // [0] in FontManager.h
	
	RawButton buttonData;
	static inline const sf::Color hoverColor_diff = sf::Color(-50, -50, -50, 0); // Color difference when hovering
public:
	sf::RectangleShape B_Box;
	sf::Text T_Text;
	// Constructors

	Button(InputWidget* parent);

	void construct(const RawButton&);

	// Text
	void setText(const std::string&); // Set the texts content
	std::string getText() const { return buttonData.text; }
	void setColor(const sf::Color&, const bool& = false); // Set the color of the text or the button fill
	sf::Color getColor(const bool& = false) const; // Get the color of the text or the button fill

	// Texture
	void setTexture(const sf::Texture&, const bool);
	void clearTexture() { return B_Box.setTexture(nullptr); };
	sf::Texture getTexture() const { return *B_Box.getTexture(); }

	// RenderInfo
	void setPosition(const sf::Vector2f& newPos) override; // Move the whole button to a new position
	void addPosition(const sf::Vector2f& deltaPos, const bool& bTickBased = true) override;
	void setRotation(const float& newRot) override;
	void setSize(const sf::Vector2f& newSize) override;

	// Alignment
	void setAlignment(const EAlignment& alignment);
	EAlignment getAlignment() const { return buttonData.alignment; }
	void setTextAlignment(const EAlignment& alignment);
	EAlignment getTextAlignment() const { return buttonData.textAlignment; }


	bool isMouseOver(const bool& = false); // Check if mouse is over button
	void onClick() const;
	void onHover();
	void onUnhover();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	RawButton getButtonData() const { return buttonData; }
};