#pragma once

#include "RenderInfo.h"
#include "Collision.h"

enum class EntityType {
	NoEntity = -1,
	Player = 0,
	Enemy,
	Projectile
};

class Entity : public IMovable, public IHasCollision
{
private:
	size_t entityID = -1; // -1 is invalid or uninitialized
	const EntityType ownType;
public:
	Entity(const EntityType&);
	~Entity() = default;

	EntityType getType() const;

	void setID(const size_t& newID);
	size_t getID() const;

	virtual void spawn() = 0;
	virtual void tick(const float& deltaTime) = 0;
};