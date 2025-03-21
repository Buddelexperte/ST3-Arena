#pragma once
#include "EntityManager.h"

size_t EntityManager::entityID = 0;

EntityManager::EntityManager()
{

}

void EntityManager::tick_kill(const float& deltaTime)
{
}

void EntityManager::tick_spawning(const float& deltaTime)
{
}

void EntityManager::tick_entities(const float& deltaTime)
{
}

void EntityManager::deleteEntity(const size_t& key)
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
    // Retrieve an enemy instance from the pool
    std::unique_ptr<Enemy> newEnemy = enemyPool.get();

    // Set the enemy's index and add it to the activeEnemies vector
    size_t enemyKey = entityID++;
    //std::cout << "Using new enemy with ID [" << enemyKey << "]" << std::endl;
    newEnemy->setID(enemyKey);
    //std::cout << "Worked" << std::endl;

    newEnemy->spawn();

    // Extract render information and pass it to the renderer
    IMovable::RenderInfo renderInfo = newEnemy->getRenderInfo();
    renderer.addEntity(renderInfo, enemyKey);

    // Actually spawn the enemy properly and update it's attributes accordingly
    activeEntities[enemyKey].ptr = (std::move(newEnemy));
    activeEntities[enemyKey].type = EntityType::Enemy;
}

void EntityManager::spawn_Proj(const IMovable::RenderInfo& renderInfo, const float& damage)
{

}

void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	renderer.draw(target, states);
}