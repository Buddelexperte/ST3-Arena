#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"
#include "Collision.h"

class GI_Arena;
class EnemyManager;

class Enemy : public ICollidable
{
private:
	GI_Arena* gameInstance;
	EnemyManager* manager;
	RNG& random = RNG::getInstance();
	Player* playerRef = nullptr;

	CollisionBox collisionBox;

	size_t enemyIndex = -1;
	RenderInfo renderInfo;

	const float speed = 100.0f;

	sf::Vector2f getNewSpawnPos() const;

	void tick_move(const float&, const RenderInfo&);
	void tick_collision(const float& deltaTime) override;
	void die();

public:
	Enemy();
	void setPlayer(Player*);
	void setID(const size_t& newIndex) { enemyIndex = newIndex; }

	void spawn();
	void tick(const float&, const RenderInfo& playerRenderInfo);

	// Collision
	ICollidable* getCollision() override { return &collisionBox; }
	virtual void onCollision(ICollidable* other) override;

	// Position
	void setPosition(const sf::Vector2f& pos);
	sf::Vector2f getPosition() const { return renderInfo.pos; }
	// Size
	void setSize(const sf::Vector2f& size);
	sf::Vector2f getSize() const { return renderInfo.size; }
	// Color
	void setColor(const sf::Color& color);
	sf::Color getColor() const { return renderInfo.color; }
	// Velocity (only get)
	sf::Vector2f getVelocity() const { return renderInfo.velocity; }

	void setRenderInfo(const RenderInfo&);
	RenderInfo getRenderInfo() const { return renderInfo; }
};