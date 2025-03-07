// UnifiedProjectile.hpp
#pragma once
#include <SFML/Graphics.hpp>

#include "RandomNumbers.h"
#include "BaseTypes.h"
#include "RenderInfo.h"
#include "Collision.h"

class GI_Arena;
class ProjectileManager;

class ProjectileSpawner : public IMovable
{
private:
    GI_Arena* gameInstance;
    ProjectileManager* manager;
    RNG& random = RNG::getInstance();

    RenderInfo baseInfo;
public:
    ProjectileSpawner();
    
    virtual void spawnProjectile();
};

// The Projectile class now inherits from IMovable and ICollidable.
class Projectile : public IMovable, public ICollidable
{
private:
    // Use CollisionBox for collision detection.
    CollisionBox collisionBox;
    bool active = true;

    size_t projectileIndex = -1;

public:
    // Constructor initializes movement info and collision box.
    Projectile()
        : collisionBox(sf::Vector2f(0.0f, .0f), sf::Vector2f(0.0f, 0.0f))
        {}

    Projectile(const RenderInfo& initRenderInfo)
        : collisionBox(initRenderInfo.pos, initRenderInfo.size)
    {
        setRenderInfo(initRenderInfo);
    }
    void setID(const size_t& newIndex) { projectileIndex = newIndex; }

    // Override ICollidable to return the projectile's collision box.
    ICollidable* getCollision() override
    {
        return &collisionBox;
    }

    void onCollision(ICollidable* other) override;

    void spawn()
    {

    }

    void tick_move(const float& deltaTime) override
    {
        sf::Vector2f delta = getVelocity() * deltaTime;
        addPosition(delta);
    }

    void tick_collision(const float& deltaTime)
    {

    }

    // Update movement and synchronize the collision box position.
    void tick(const float& deltaTime)
    {
        tick_move(deltaTime);

        tick_collision(deltaTime);
    }

    bool isActive() const { return active; }
    void deactivate() { active = false; }

    void setRenderInfo(const RenderInfo& newRenderInfo) override
    {
        IMovable::setRenderInfo(newRenderInfo);
        collisionBox.setPos(newRenderInfo.pos);
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

};
