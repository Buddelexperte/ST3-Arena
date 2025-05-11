#pragma once
// Own libraries
#include "FontManager.h"
#include "WidgetBase.h"
#include <functional>

#include "C_ColorFade.h" // For clean color fading

// Pos, Size, Color, TextSize, TextString, TextColor, Alignment, TextAlignment
struct RawButton
{
	sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f size = sf::Vector2f(100.0f, 100.0f);
	sf::Color color = sf::Color::White;
	unsigned int textSize = 0;
	std::string text = "";
	sf::Color textColor = sf::Color::White;
	EAlignment alignment = EAlignment::CENTER;
	EAlignment textAlignment = EAlignment::CENTER;
};

class Button : public WidgetElement
{
private:
	FontManager::Font font = FontManager::Font::BUTTON_FONT; // [0] in FontManager.h
	
	RawButton buttonData;
	static inline const sf::Color HOVER_COLOR_DELTA = sf::Color(50, 50, 50, 0); // Color difference when hovering
	bool bHovered = false;

	bool bEnabled = true;

	void playButtonSound() const;

	sf::RectangleShape B_Box;
	ColorFade boxFade;

	sf::Text T_Text;
	ColorFade textFade;

	// Button Animations
	virtual void start_onHoverAnim() override; // On Hover
	virtual void tick_onHoverAnim(const float&) override;
	virtual void start_onUnhoverAnim() override; // On Unhover
	virtual void tick_onUnhoverAnim(const float&) override;
	virtual void start_onClickAnim() override; // On Click
	virtual void tick_onClickAnim(const float&) override;

public:
	// Constructors
	Button(InputWidget* parent);

	void construct();
	void construct(const RawButton&);
	void construct(const RawButton&, const bool);

	// Text
	void setText(const std::string&); // Set the texts content
	std::string getText() const { return buttonData.text; }

	// Color
	void setColor(const sf::Color&) override; // Set the color of the text or the button fill
	sf::Color getColor() const override; // Get the color of the text or the button fill
	void setTextColor(const sf::Color&); // Set the color of the text or the button fill
	sf::Color getTextColor() const; // Set the color of the text or the button fill
	
	// Text size
	void setTextSize(const unsigned int&);
	unsigned int getTextSize() const;

	// Texture
	void setTexture(const sf::Texture&, const bool);
	void clearTexture() { return B_Box.setTexture(nullptr); };
	sf::Texture getTexture() const { return *B_Box.getTexture(); }

	// RenderInfo
	void setPosition(const sf::Vector2f& newPos) override; // Move the whole button to a new position
	void addPosition(const sf::Vector2f& deltaPos, const bool& bTickBased = true) override;
	void setRotation(const float& newRot) override;
	void setSize(const sf::Vector2f& newSize) override;

	void setEnabled(const bool);
	bool getEnabled() const { return bEnabled; }

	// Alignment
	void setAlignment(const EAlignment& alignment);
	EAlignment getAlignment() const { return buttonData.alignment; }
	void setTextAlignment(const EAlignment& alignment);
	EAlignment getTextAlignment() const { return buttonData.textAlignment; }

	RawButton getButtonData() const { return buttonData; }

	bool isMouseOver(const bool& = false); // Check if mouse is over button
	
	// Callback functions
	std::function<void()> onClick = nullptr; 
	std::function<void()> onHover = nullptr;
	std::function<void()> onUnhover = nullptr;

};