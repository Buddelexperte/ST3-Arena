#pragma once

#include "WidgetBase.h"
#include "C_ColorFade.h"
#include "FontManager.h"

struct RawText
{
	sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
	sf::Color color = sf::Color::White;
	unsigned int fontSize = 0;
	std::string text = "";
	EAlignment alignment = EAlignment::CENTER;
};

class Text : virtual public WidgetElement
{
private:
	RawText textData;

protected:
	sf::Text T_Text;
	ColorFade textFade;

	FontManager::Font font = FontManager::Font::BUTTON_FONT; // [0] in FontManager.h

public:
	// Constructors
	Text(InputWidget* parent);

	void construct();
	void construct(const RawText&);

	// Text
	void setText(const std::string&);
	std::string getText() const { return textData.text; }
	void setTextSize(const unsigned int&);
	unsigned int getTextSize() const;

	// RenderInfo
	void setPosition(const sf::Vector2f& newPos) override; // Move the whole button to a new position
	void addPosition(const sf::Vector2f& deltaPos, const bool& bTickBased = true) override;
	void setRotation(const float& newRot) override;
	void setColor(const sf::Color&) override;

	// Alignment
	void setAlignment(const EAlignment& alignment);
	EAlignment getAlignment() const { return textData.alignment; }

	RawText getBorderData() const { return textData; }
};