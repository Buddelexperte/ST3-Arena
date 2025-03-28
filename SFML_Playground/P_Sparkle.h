#pragma once

#include "Entity.h"
#include "Lifetime.h"

class P_Sparkle : public Entity, public IHasLifetime
{
private:
	// EXperience Orb variables, need to be moved over TODO
	sf::Vector2f startSize;
	float rotationSpeed = 0.0f;

	CollisionBox collisionBox;
	
	void tick_move(const float&) override;

	virtual void timeout() override
	{
		kill_self();
	}

protected:
	float value = 0.0f;

public:
	P_Sparkle();
	~P_Sparkle() = default;

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