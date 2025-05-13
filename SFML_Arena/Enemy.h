#pragma once
#include <SFML/Graphics.hpp>

#include "BaseTypes.h"
#include "RandomNumbers.h"
#include "Health.h"
#include "Collision.h"
#include "RenderInfo.h"
#include "Entity.h"

class Enemy : 
	public Entity, public IHasDamage, public IHasHealth
{
private:
	CollisionBox collisionBox;

	const float WALK_SPEED = RNG::floatInRange(80.0f, 100.0f);

	const float experienceValue = RNG::floatInRange(1.0f, 5.0f);

	using IMovable::setVelocity; // Make this function private for safety reasons

	virtual void tick_move(const float&) override;
	virtual void tick_health(const float&) override;
	void spawnDeathParticle();
	void spawnExperience();
	void kill_self(const bool&);

public:
	Enemy();
	~Enemy() = default;

	void spawn( SpawnInformation) override;
	void tick(const float&) override;
	void releaseToPool() override;

	// RenderInfo general
	void setRenderInfo(const RenderInfo& newRenderInfo) override;
	// Position
	void setPosition(const sf::Vector2f& pos) override;
	void addPosition(const sf::Vector2f& delta, const bool& bVelocityBased = true) override;
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