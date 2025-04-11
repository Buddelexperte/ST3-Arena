#pragma once

#include "WidgetBase.h"

class BackgroundElement 
	: public WidgetElement
{
private:
	// Components
	sf::VertexArray background;  // 4 vertices for the rectangle (quad), specified in Widget Constructor
	sf::Texture backgroundTexture;

	// Info
	sf::Vector2f backgroundPos;

	// Config
	static constexpr float TILING_SCALE = 0.1f; // Larger = more tiling
	static constexpr float parallaxStrength = 1.0f; // Adjust this factor for stronger/weaker parallax

	void updateVertexArray();

public:
	BackgroundElement(InputWidget*);
	~BackgroundElement() = default;

	void construct() override;
	void tick(const float& deltaTime) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::VertexArray& getVertexArray()
	{
		return background;
	}
};