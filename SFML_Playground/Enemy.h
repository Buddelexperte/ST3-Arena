#pragma once
#include <SFML/Graphics.hpp>

#include "BaseTypes.h"
#include "RandomNumbers.h"
#include "Health.h"
#include "Collision.h"
#include "RenderInfo.h"
#include "Entity.h"

// Forward Declarations
class GI_Arena;
class EnemyManager;

class Enemy : 
	public Entity
{
private:
	CollisionBox collisionBox;

	const float WALK_SPEED = RNG::floatInRange(80.0f, 100.0f);
	const float ROT_LERP_MULTIPLIER = RNG::floatInRange(0.3f, 0.8f);
	const float ROT_LERP = LERP_SMOOTHNESS * ROT_LERP_MULTIPLIER;

	using IMovable::setVelocity; // Make this function private for safety reasons

	void tick_move(const float&) override;
	void spawnExperience();
	void kill_self(const bool&);

public:
	Enemy();
	~Enemy() = default;

	void spawn(const SpawnInformation&) override;
	void tick(const float&) override;
	void releaseToPool() override;

	// RenderInfo general
	void setRenderInfo(const RenderInfo& newRenderInfo) override;
	// Position
	void setPosition(const sf::Vector2f& pos) override;
	void addPosition(const sf::Vector2f& delta) override;
	// Size
	void setSize(const sf::Vector2f& size) override;
	// Color
	void setColor(const sf::Color& color) override;

	// Collision
	Collidable* getCollision() override
		{ return &collisionBox; }

	virtual void onCollision(IHasCollision* other) override;

	void collideWithPlayer(Player& player) override;
	void collideWithProjectile(Projectile& projectile) override;
};