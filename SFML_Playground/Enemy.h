#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"
#include "Player.h"
#include "Health.h"
#include "Collision.h"
#include "RenderInfo.h"

// Forward Declarations
class GI_Arena;
class EnemyManager;

class Enemy : 
	public IHasCollision, public IMovable, IHasHealth
{
private:
	CollisionBox collisionBox;
	ValueBar healthBar;
	virtual ValueBar& getValueBar() override
		{ return healthBar; }

	sf::Vector2f getNewSpawnPos() const;
	size_t enemyIndex = -1;

	const float WALK_SPEED = RNG::floatInRange(80.0f, 100.0f);
	const float ROT_LERP_MULTIPLIER = RNG::floatInRange(0.3, 0.8f);
	const float ROT_LERP = LERP_SMOOTHNESS * ROT_LERP_MULTIPLIER;

	using IMovable::setVelocity; // Make this function private for safety reasons

	float damage = 0.1f;

	void tick_move(const float&) override;
	void kill_self(const bool&) const;

public:
	Enemy();
	~Enemy();

	void setID(const size_t& newIndex)
		{ enemyIndex = newIndex; }

	void spawn();
	void tick(const float&);

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

	float getDamage() const
		{ return damage; }
};