// UnifiedProjectile.hpp
#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"
#include "BaseTypes.h"
#include "Entity.h"
#include "CollisionManager.h"


class ProjectileSpawner 
    : public IMovable
{
protected:
    static inline RNG& random = RNG::getInstance();

    // Offset distance from the player's center
    static constexpr float SIM_ARM_LENGTH = 25.0f; // Adjust this value as needed

private:
    const RenderInfo baseInfo = {
        {0.0f, 0.0f}, // Default offset
        {50.0f, 50.0f}, // Default projectile size
        0.0f, // Default rotation offset
        {1500.0f, 1500.0f}, // Default projectile speed (velocity later multiplied with direction)
        sf::Color::White // Default projectile Color
    };

	float damage = 1.0f;

    void spawnParticle(const SpawnInformation&);

public:
    ProjectileSpawner(const RenderInfo& baseInfo, const float& damage);
	virtual ~ProjectileSpawner() = default;

    virtual void shoot() = 0;
    void shoot(const SpawnInformation&);

	float getDamage() const
	{
		return damage;
	}

    void setDamage(const float& newDamage)
    {
		damage = newDamage;
    }
};

#include "Lifetime.h"

// The Projectile class now inherits from IMovable and ICollidable
class Projectile : 
    public Entity, public IHasLifetime, public IHasDamage
{
private:
    static constexpr float MAX_LIFETIME = 5.0f;

    CollisionBox collisionBox;

    void tick_move(const float&) override;

    virtual void timeout() override
    {
        kill_self();
    }

public:
    // Constructor initializes movement info and collision box.
    Projectile();
    Projectile(const RenderInfo& initRenderInfo);
    ~Projectile() = default;

    // Override Collidable to return the projectile's collision box.
    Collidable* getCollision() override
        { return &collisionBox; }

    void onCollision(IHasCollision* other) override;
    void collideWithEnemy(Enemy& enemy) override;

    void spawn( SpawnInformation spawnInfo) override;
    void tick(const float& deltaTime) override;
    void releaseToPool() override;

    void setRenderInfo(const RenderInfo& newRenderInfo) override
    {
        Entity::setRenderInfo(newRenderInfo);
        collisionBox.setPos(newRenderInfo.pos);
        collisionBox.setSize(newRenderInfo.size);
    }

    void setPosition(const sf::Vector2f& newPos) override
    {
        Entity::setPosition(newPos);
        collisionBox.setPos(newPos);
    }


    void addPosition(const sf::Vector2f& deltaPos) override
    {
        Entity::addPosition(deltaPos);
        collisionBox.setPos(collisionBox.getPos() + deltaPos);
    }

    void setSize(const sf::Vector2f& newSize) override
    {
        Entity::setSize(newSize);
        collisionBox.setSize(newSize);
    }
};
