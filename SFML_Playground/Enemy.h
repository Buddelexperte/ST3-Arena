#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"

class GI_Arena;
class EnemyManager;

class Enemy : public sf::Drawable
{
private:
	GI_Arena* gameInstance;
	EnemyManager* manager;

	size_t enemyIndex = -1;

	RNG& random = RNG::getInstance();

	sf::RectangleShape shape;
	sf::Vector2f position;
	sf::Vector2f velocity;

	sf::Vector2f getNewSpawnPos() const;
public:
	struct EnemyRenderInfo {
		sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f size = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
		sf::Color color = sf::Color::Transparent;
	};

	Enemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);

	void spawn();
	void spawn(const sf::Vector2f&);

	void setIndex(const size_t& newIndex) { enemyIndex = newIndex; }

	void tick(const float& deltaTime);

	void setPosition(const sf::Vector2f& pos);
	sf::Vector2f getPosition() const {return position;}
	void setSize(const sf::Vector2f& size);
	sf::Vector2f getSize() const {return shape.getSize();}
	void setColor(const sf::Color& color);
	sf::Color getColor() const {return shape.getFillColor();}

	EnemyRenderInfo getRenderInfo() const
	{
		return EnemyRenderInfo(getPosition(), getSize(), sf::Vector2f(0.0f, 0.0f), getColor());
	}

	sf::Vector2f getVelocity() const { return velocity; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};