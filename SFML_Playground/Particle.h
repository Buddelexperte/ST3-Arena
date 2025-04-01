#pragma once

#include "Entity.h"
#include "Lifetime.h"

class Particle
	: public Entity, public IHasLifetime
{
private:
	CollisionBox collisionBox;

	void tick_move(const float&) override = 0;

	virtual void timeout() override
	{
		kill_self();
	}


public:
	Particle();
	~Particle() = default;

	virtual Collidable* getCollision() override
	{
		return &collisionBox;
	}

	void spawn(const SpawnInformation&) override;
	void tick(const float&) override;
	void releaseToPool() override;
};