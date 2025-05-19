#pragma once

#include "RenderInfo.h"
#include "Collision.h"
#include "Health.h"
#include "Damage.h"
#include "GenericPool.h"

#include "BaseTypes.h"

struct SpawnInformation
{
	IMovable::RenderInfo renderInfo;
	float health;
	float damage;
	int flag = 0;
};

class Entity 
	: public IMovable, public IHasCollision
{
private:
	size_t entityID = -1; // -1 is invalid or uninitialized
	const EntityType ownType;

protected:
	float timeAlive = 0.0f;
	void resetTimeAlive()
	{
		timeAlive = 0.0f;
	}
	void updateTimeAlive(const float& deltaTime)
	{
		timeAlive += deltaTime;
	}

	virtual void kill_self();

public:
	Entity(const EntityType&);
	virtual ~Entity() = default;

	EntityType getType() const;

	void setID(const size_t& newID);
	size_t getID() const;

	// Using the CollisioNManager
	void enableCollision();
	void disableCollision();
	// IHasCollision
	virtual Collidable* getCollision() override = 0;

	virtual void spawn(SpawnInformation) = 0;
	virtual void tick(const float& deltaTime) = 0;
	virtual void releaseToPool() = 0;

	// RenderInfo + CallUpdate
	void setRenderInfo(const RenderInfo&) override;
	void setPosition(const sf::Vector2f&) override;
	void addPosition(const sf::Vector2f&, const bool& bVelocityBased = true) override;
	void setRotation(const float&) override;
	void setSize(const sf::Vector2f&) override;
	void setColor(const sf::Color&) override;
	void setVelocity(const sf::Vector2f&) override;
};