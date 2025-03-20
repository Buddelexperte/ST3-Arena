#pragma once

#include "Entity.h"
#include "CollisionManager.h"

Entity::Entity(const EntityType& type) 
	: ownType(type)
{
}

EntityType Entity::getType() const
{
	return ownType;
}

void Entity::setID(const size_t& newID)
{
	entityID = newID;
}

size_t Entity::getID() const
{
	return entityID;
}