#pragma once

#include "Entity.h"

class Pickup : public Entity
{
private:
	// EXperience Orb variables, need to be moved over TODO
	sf::Vector2f startSize;
	float rotationSpeed = 0.0f;

	CollisionBox collisionBox;
	
	ValueBar lifeTime;

	void tick_move(const float&) override;
	void tick_lifetime(const float&);

protected:
	float value = 0.0f;

public:
	Pickup();
	~Pickup() = default;

	virtual Collidable* getCollision() override
		{ return &collisionBox; }

	void setValue(const float&);
	float getValue() const;

	void spawn(const SpawnInformation&) override;
	void tick(const float&) override;
	void releaseToPool() override;

	virtual void onCollision(IHasCollision* other) override;

	void setSize(const sf::Vector2f&) override;
};