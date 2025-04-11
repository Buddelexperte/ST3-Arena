#include "Background.h"


BackgroundElement::BackgroundElement(InputWidget*)
	: WidgetElement(parent),
	background(sf::Quads, 4)
{
	// Load texture
	if (!backgroundTexture.loadFromFile("Content/Textures/cobblestone_mossy.png"))
	{
		std::cerr << "Error loading the background texture!" << std::endl;
	}
	backgroundTexture.setRepeated(true);

	// No shape to draw, background vertext array is handled differently
	shapes = { };

	updateVertexArray();

	window->draw(background, &backgroundTexture);

}

void BackgroundElement::construct()
{

}

void BackgroundElement::tick(const float& deltaTime)
{
	updateVertexArray();
}

void BackgroundElement::updateVertexArray()
{
	// This creates the parallax effect: background moves as the sf::View moves
	backgroundPos = viewCenter * parallaxStrength; // Adjust this factor for stronger/weaker parallax

	// Background will cover the entire view area
	background[0].position = sf::Vector2f(viewCenter.x - viewSize.x / 2.0f, viewCenter.y - viewSize.y / 2.0f);
	background[1].position = sf::Vector2f(viewCenter.x + viewSize.x / 2.0f, viewCenter.y - viewSize.y / 2.0f);
	background[2].position = sf::Vector2f(viewCenter.x + viewSize.x / 2.0f, viewCenter.y + viewSize.y / 2.0f);
	background[3].position = sf::Vector2f(viewCenter.x - viewSize.x / 2.0f, viewCenter.y + viewSize.y / 2.0f);

	float textureOffsetX = static_cast<float>(fmod(backgroundPos.x * TILING_SCALE, backgroundTexture.getSize().x));
	float textureOffsetY = static_cast<float>(fmod(backgroundPos.y * TILING_SCALE, backgroundTexture.getSize().y));

	background[0].texCoords = sf::Vector2f(textureOffsetX, textureOffsetY);  // Top-left
	background[1].texCoords = sf::Vector2f(textureOffsetX + viewSize.x * TILING_SCALE, textureOffsetY);  // Top-right
	background[2].texCoords = sf::Vector2f(textureOffsetX + viewSize.x * TILING_SCALE, textureOffsetY + viewSize.y * TILING_SCALE);  // Bottom-right
	background[3].texCoords = sf::Vector2f(textureOffsetX, textureOffsetY + viewSize.y * TILING_SCALE);  // Bottom-left
}

void BackgroundElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background, &backgroundTexture);
}
