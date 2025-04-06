#pragma once

#include "Pickup.h"

class PU_ExperienceOrb : public Pickup
{
private:

	unsigned int experienceValue = 0;

	CollisionBox collisionBox;

	virtual sf::Color getCorrectColor(const unsigned int& experienceGroup) const;
	virtual sf::Vector2f getCorrectSize(const unsigned int& experienceGroup) const;

	virtual void tick_move(const float& deltaTime) override;

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

	virtual void spawn(SpawnInformation) override;
	virtual void tick(const float& deltaTime) override;
	virtual void releaseToPool() override;
	
	virtual void addPosition(const sf::Vector2f& delta, const bool& bVelocityBased = true) override;

	unsigned int getExp() const;

	virtual void setRenderInfo(const IMovable::RenderInfo&) override;
	virtual void setPosition(const sf::Vector2f&) override;
	virtual void setSize(const sf::Vector2f&) override;
};