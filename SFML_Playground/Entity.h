#pragma once

#include "RenderInfo.h"
#include "Collision.h"
#include "Health.h"
#include "Damage.h"

enum class EntityType {
	NoEntity = -1,
	Player = 0,
	Enemy,
	Projectile
};

struct SpawnInformation
{
	IMovable::RenderInfo renderInfo;
	float health;
	float damage;
};

class Entity 
	: public IMovable, public IHasCollision, public IHasHealth, public IHasDamage
{
private:
	size_t entityID = -1; // -1 is invalid or uninitialized
	const EntityType ownType;

	ValueBar healthBar;
	virtual ValueBar& getHealthBar() override
	{
		return healthBar;
	}

public:
	Entity(const EntityType&);
	~Entity() = default;

	EntityType getType() const;

	void setID(const size_t& newID);
	size_t getID() const;

	virtual void spawn(const SpawnInformation&) = 0;
	virtual void tick(const float& deltaTime) = 0;

	// RenderInfo + CallUpdate
	void setRenderInfo(const RenderInfo&) override;
	void setPosition(const sf::Vector2f&) override;
	void addPosition(const sf::Vector2f&) override;
	void setRotation(const float&) override;
	void setSize(const sf::Vector2f&) override;
	void setColor(const sf::Color&) override;
	void setVelocity(const sf::Vector2f&) override;
};