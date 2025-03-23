#pragma once

#include "Entity.h"
#include "EntityManager.h"

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

void Entity::setRenderInfo(const RenderInfo& renderInfo)
{
	IMovable::setRenderInfo(renderInfo);
}

void Entity::setPosition(const sf::Vector2f& newPos)
{
	IMovable::setPosition(newPos);
	entityManager().callUpdate(entityID, InfoType::POSITION);
}

void Entity::addPosition(const sf::Vector2f& delta)
{
	IMovable::addPosition(delta);
	entityManager().callUpdate(entityID, InfoType::POSITION);
}

void Entity::setRotation(const float& newRot)
{
	IMovable::setRotation(newRot);
	entityManager().callUpdate(entityID, InfoType::ROTATION);
}

void Entity::setSize(const sf::Vector2f& newSize)
{
	IMovable::setSize(newSize);
	entityManager().callUpdate(entityID, InfoType::SIZE);
}

void Entity::setColor(const sf::Color& newColor)
{
	IMovable::setColor(newColor);
	entityManager().callUpdate(entityID, InfoType::COLOR);
}

void Entity::setVelocity(const sf::Vector2f& newVelo)
{
	IMovable::setVelocity(newVelo);
	entityManager().callUpdate(entityID, InfoType::VELOCITY);
}