#pragma once

#include "Pickup.h"

class PU_ExperienceOrb : public Pickup
{
private:
	virtual void tick_move(const float& deltaTime) override;

	float experienceValue = 0.0f;

	CollisionBox collisionBox;

protected:
	virtual void onPickup() override;

public:
	PU_ExperienceOrb()
		: Pickup(),
		collisionBox(this, getPosition(), sf::Vector2f(10.0f, 10.0f))
	{

	}
	~PU_ExperienceOrb() = default;

	// IHasCollision
	virtual Collidable* getCollision() override
	{
		return &collisionBox;
	}

	virtual void spawn(const SpawnInformation& spawnInfo) override;
	virtual void tick(const float& deltaTime) override;
	virtual void releaseToPool() override;
	
	virtual void addPosition(const sf::Vector2f& delta) override;

	void setExp(const float&);
	float getExp() const;
};