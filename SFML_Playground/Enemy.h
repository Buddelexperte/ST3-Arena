#pragma once
#include <SFML/Graphics.hpp>

class GI_Arena;

class Enemy : public sf::Drawable
{
private:
    GI_Arena& gameInstance;

	sf::RectangleShape shape;
	sf::Vector2f velocity;
public:
	Enemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);

	void tick(const float& deltaTime);

	void setPosition(const sf::Vector2f& pos);
	void setSize(const sf::Vector2f& size);
	void setColor(const sf::Color& color);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};