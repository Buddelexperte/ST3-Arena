#pragma once
#include "EntityManager.h"

EntityManager::EntityManager()
{

}

size_t EntityManager::getNumEntites() const
{
	return activeEntities.size();
}

void EntityManager::tick(const float& deltaTime)
{

}

void EntityManager::callDelete(const size_t& key)
{

}

void EntityManager::deleteAll()
{

}

void EntityManager::callUpdate(const size_t& key, const InfoType& infoType)
{

}

void EntityManager::spawn_Player(const IMovable::RenderInfo& renderInfo)
{

}

void EntityManager::spawn_Enemy(const IMovable::RenderInfo& renderInfo)
{

}

void EntityManager::spawn_Proj(const IMovable::RenderInfo& renderInfo, const float& damage)
{

}

void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	renderer.draw(target, states);
}