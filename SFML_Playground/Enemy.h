#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"

class GI_Arena;
class EnemyManager;

class Enemy
{
public:
	struct EnemyRenderInfo {
		sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f size = sf::Vector2f(100.0f, 100.0f);
		sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
		sf::Color color = sf::Color::White;
	};
private:
	GI_Arena* gameInstance;
	EnemyManager* manager;
	RNG& random = RNG::getInstance();

	size_t enemyIndex = -1;
	EnemyRenderInfo renderInfo;

	const float speed = 100.0f;

	sf::Vector2f getNewSpawnPos() const;

	void tick_collision(const float&);
	void tick_move(const float&);
	void die();
public:

	Enemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);

	void setIndex(const size_t& newIndex) { enemyIndex = newIndex; }

	void spawn();
	void spawn(const sf::Vector2f&);


	void tick(const float&);

	// Position
	void setPosition(const sf::Vector2f& pos);
	sf::Vector2f getPosition() const {return renderInfo.pos;}
	// Size
	void setSize(const sf::Vector2f& size);
	sf::Vector2f getSize() const {return renderInfo.size;}
	// Color
	void setColor(const sf::Color& color);
	sf::Color getColor() const {return renderInfo.color;}
	// Velocity (only get)
	sf::Vector2f getVelocity() const { return renderInfo.velocity; }

	EnemyRenderInfo getRenderInfo() const
	{
		return renderInfo;
	}
};