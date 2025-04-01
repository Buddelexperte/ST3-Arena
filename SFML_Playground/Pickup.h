#pragma once

#include "Entity.h"

class Pickup : public Entity
{
protected:
    virtual void onPickup() = 0;

public:
    Pickup();
    ~Pickup() = default;

    virtual Collidable* getCollision() override = 0;

    virtual void spawn(const SpawnInformation& spawnInfo) = 0;
    virtual void tick(const float& deltaTime) override = 0;
    virtual void releaseToPool() override = 0;


    void onCollision(IHasCollision* other) override;
    void collideWithPlayer(Player& player) override;
};
