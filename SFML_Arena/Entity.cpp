#pragma once

#include "Entity.h"
#include "EntityManager.h"

void Entity::enableCollision()
{
	CollisionManager::getInstance().registerEntity(getCollision(), getType());
}

void Entity::disableCollision()
{ 
	CollisionManager::getInstance().unregisterEntity(getCollision()->getCollisionID(), getType());
}

void Entity::kill_self()
{
	EntityManager::getInstance().callDelete(getID());
}

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
	if (renderInfo == getRenderInfo())
		return;

	IMovable::setRenderInfo(renderInfo);
}

void Entity::setPosition(const sf::Vector2f& newPos)
{
	if (newPos == getPosition())
		return;

	IMovable::setPosition(newPos);
	EntityManager::getInstance().callUpdate(entityID, InfoType::POSITION);
}

void Entity::addPosition(const sf::Vector2f& delta, const bool& bVelocityBased)
{
	if (delta == sf::Vector2f(0.0f, 0.0f))
		return;

	IMovable::addPosition(delta);
	if (!bVelocityBased)
		EntityManager::getInstance().callUpdate(entityID, InfoType::POSITION);
}

void Entity::setRotation(const float& newRot)
{
	if (newRot == getRotation())
		return;

	IMovable::setRotation(newRot);
	EntityManager::getInstance().callUpdate(entityID, InfoType::ROTATION);
}

void Entity::setSize(const sf::Vector2f& newSize)
{
	if (newSize == getSize())
		return;

	IMovable::setSize(newSize);
	EntityManager::getInstance().callUpdate(entityID, InfoType::SIZE);
}

void Entity::setColor(const sf::Color& newColor)
{
	if (newColor == getColor())
		return;

	IMovable::setColor(newColor);
	EntityManager::getInstance().callUpdate(entityID, InfoType::COLOR);
}

void Entity::setVelocity(const sf::Vector2f& newVelo)
{
	if (newVelo == getVelocity())
		return;

	IMovable::setVelocity(newVelo);
	EntityManager::getInstance().callUpdate(entityID, InfoType::VELOCITY);
}