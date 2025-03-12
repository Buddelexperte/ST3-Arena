// UnifiedProjectile.hpp
#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"
#include "BaseTypes.h"
#include "RenderInfo.h"
#include "CollisionManager.h"

class GI_Arena;
class ProjectileManager;

class ProjectileSpawner : public IMovable
{
protected:
    GI_Arena* gameInstance;
    ProjectileManager* manager;
    RNG& random = RNG::getInstance();

private:
    const RenderInfo baseInfo = {
        {0.0f, 0.0f}, // Default offset
        {50.0f, 50.0f}, // Default projectile size
        0.0f, // Default rotation offset
        {1500.0f, 1500.0f}, // Default projectile speed (velocity later multiplied with direction)
        sf::Color(100, 100, 100, 255) // Default projectile Color
    };

	const float damage = 1.0f;

public:
    ProjectileSpawner(const RenderInfo& baseInfo, const float& damage);
	virtual ~ProjectileSpawner() = default;

    virtual void shoot();
};

// The Projectile class now inherits from IMovable and ICollidable
class Projectile : public IMovable, public IHasCollision
{
private:
    const float maxLifetime = 5.0f;
    float lifetimeLeft = maxLifetime;

    float damage = 0.0f;

    CollisionBox collisionBox;

    size_t projectileID = -1;

    void kill_self() const;

public:
    // Constructor initializes movement info and collision box.
    Projectile();
    Projectile(const RenderInfo& initRenderInfo);
    ~Projectile();

    void setID(const size_t& newIndex) { projectileID = newIndex; }

    // Override Collidable to return the projectile's collision box.
    Collidable* getCollision() override
        { return &collisionBox; }

    void onCollision(IHasCollision* other) override;

    void collideWithEnemy(Enemy& enemy) override;

    void spawn()
    {
        CollisionManager::getInstance().registerProjectile(getCollision()); 
        lifetimeLeft = maxLifetime;
    }

    void tick_move(const float&) override;

    void tick_lifetime(const float&);

    // Update movement and synchronize the collision box position.
    void tick(const float& deltaTime)
    {
        tick_move(deltaTime);

        tick_lifetime(deltaTime);
    }

    void setRenderInfo(const RenderInfo& newRenderInfo) override
    {
        IMovable::setRenderInfo(newRenderInfo);
        collisionBox.setPos(newRenderInfo.pos);
        collisionBox.setSize(newRenderInfo.size);
    }

    void setPosition(const sf::Vector2f& newPos) override
    {
        IMovable::setPosition(newPos);
        collisionBox.setPos(newPos);
    }


    void addPosition(const sf::Vector2f& deltaPos) override
    {
        IMovable::addPosition(deltaPos);
        collisionBox.setPos(collisionBox.getPos() + deltaPos);
    }

    void setSize(const sf::Vector2f& newSize) override
    {
        IMovable::setSize(newSize);
        collisionBox.setSize(newSize);
    }

    void setDamage(const float& newDamage)
	    { damage = newDamage; }

    float getDamage() const
        { return damage; }
};
