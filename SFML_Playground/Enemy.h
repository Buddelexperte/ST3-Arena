#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"
#include "Player.h"
#include "CollisionManager.h"
#include "RenderInfo.h"

class GI_Arena;
class EnemyManager;

class Enemy : public IHasCollision, public IMovable
{
private:
	GI_Arena* gameInstance;
	EnemyManager* manager;
	Player* playerRef = nullptr;
	RNG& random = RNG::getInstance();

	CollisionBox collisionBox;

	size_t enemyIndex = -1;

	const float WALK_SPEED = random.floatInRange(80.0f, 100.0f);
	const float ROT_LERP_MULTIPLIER = random.floatInRange(0.3, 0.8f);
	const float ROT_LERP = LERP_SMOOTHNESS * ROT_LERP_MULTIPLIER;

	using IMovable::setVelocity; // Make this function private for safety reasons

	sf::Vector2f getNewSpawnPos() const;

	void tick_move(const float&) override;
	void kill_self();

public:
	Enemy();
	~Enemy();

	void setPlayer(Player*);
	void setID(const size_t& newIndex)
		{ enemyIndex = newIndex; }

	void spawn();
	void tick(const float&);

	// Collision
	Collidable* getCollision() override 
		{ return &collisionBox; }
	virtual void onCollision(Collidable* other) override;

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