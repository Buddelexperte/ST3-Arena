#pragma once

#include "WidgetBase.h"
#include "C_ColorFade.h"

struct RawBorder
{
	sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f size = sf::Vector2f(100.0f, 100.0f);
	sf::Color color = sf::Color::White;
	EAlignment alignment = EAlignment::CENTER;
};

class Border : public WidgetElement
{
private:
	RawBorder borderData;

protected:
	sf::RectangleShape B_Box;
	ColorFade boxFade;

public:
	Border(InputWidget* parent);

	void construct();
	void construct(const RawBorder&);

	// Texture
	void setTexture(const sf::Texture&, const bool);
	void clearTexture() { return B_Box.setTexture(nullptr); };
	sf::Texture getTexture() const { return *B_Box.getTexture(); }

	// RenderInfo
	void setPosition(const sf::Vector2f& newPos) override; // Move the whole button to a new position
	void addPosition(const sf::Vector2f& deltaPos, const bool& bTickBased = true) override;
	void setRotation(const float& newRot) override;
	void setColor(const sf::Color&) override;
	void setSize(const sf::Vector2f& newSize) override;

	bool isMouseOver(const bool&) override;

	// Alignment
	void setAlignment(const EAlignment& alignment);
	EAlignment getAlignment() const { return borderData.alignment; }

	RawBorder getBorderData() const { return borderData; }
};

