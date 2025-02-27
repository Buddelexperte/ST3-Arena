#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"
#include "Player.h"
// Parent Classes
#include "Collision.h"
#include "RenderInfo.h"

class GI_Arena;
class EnemyManager;

class Enemy : public ICollidable, public IMovable
{
private:
	GI_Arena* gameInstance;
	EnemyManager* manager;
	Player* playerRef = nullptr;
	RNG& random = RNG::getInstance();

	CollisionBox collisionBox;

	size_t enemyIndex = -1;

	const float speed = random.floatInRange(80.0f, 100.0f);
	const float ROT_LERP_MULTIPLIER = random.floatInRange(0.3, 0.8f);
	const float ROT_LERP = LERP_SMOOTHNESS * ROT_LERP_MULTIPLIER;

	using IMovable::setVelocity; // Make this function private for safety reasons

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

	// RenderInfo general
	void setRenderInfo(const RenderInfo& newRenderInfo) override;
	// Position
	void setPosition(const sf::Vector2f& pos) override;
	void addPosition(const sf::Vector2f& delta) override;
	// Size
	void setSize(const sf::Vector2f& size) override;
	// Color
	void setColor(const sf::Color& color) override;
};