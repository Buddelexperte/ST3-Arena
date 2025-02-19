#pragma once
#include <iostream>
#include <thread>

#include "EnemyManager.h" // Header File

int EnemyManager::enemyID = 0;

EnemyManager::EnemyManager()
	: enemyPool(), enemyRenderer(), gameInstance(GI_Arena::getInstance())
{}

void EnemyManager::setPlayer(Player* newPlayer)
{
    for (auto& enemy : activeEnemies)
    {
        enemy.second->setPlayer(newPlayer);
    }
}

int EnemyManager::getNumActiveEnemies() const
{
	return static_cast<int>(activeEnemies.size());
}

void EnemyManager::spawnEnemy(const Enemy::RenderInfo& renderInfo)
{
    // Retrieve an enemy instance from the pool
    std::unique_ptr<Enemy> newEnemy = enemyPool.get();

    // Set the enemy's index and add it to the activeEnemies vector
    size_t enemyKey = enemyID++;
    newEnemy->setID(enemyKey);

    newEnemy->setRenderInfo(renderInfo);

    // Extract render information and pass it to the renderer
    enemyRenderer.addEnemy(renderInfo, enemyKey);

    // Actually spawn the enemy properly and update it's attributes accordingly
    activeEnemies[enemyKey] = (std::move(newEnemy));
    activeEnemies[enemyKey]->spawn();
}

void EnemyManager::deleteEnemy(const size_t& key)
{
    enemyPool.release(std::move(activeEnemies[key]));
    
    activeEnemies.erase(key);

    enemyRenderer.removeEnemy(key);
}

void EnemyManager::callDelete(const size_t key)
{
    pendingKill.insert(key);
}

void EnemyManager::callUpdate(const size_t& key, const InfoType& updateFlags = InfoType::EMPTY_INFO)
{
    if (updateFlags & InfoType::POSITION) { // Check if POSITION flag is set
        enemyRenderer.setPosition(key, activeEnemies[key]->getPosition());
    }
    if (updateFlags & InfoType::SIZE) { // Check if SIZE flag is set
        enemyRenderer.setSize(key, activeEnemies[key]->getSize());
    }
    if (updateFlags & InfoType::COLOR) { // Check if COLOR flag is set
        enemyRenderer.setColor(key, activeEnemies[key]->getColor());
    }
}

void EnemyManager::tick_kill(const float& deltaTime)
{
    for (const size_t& key : pendingKill)
    {
        deleteEnemy(key);
    }
    pendingKill.clear();
}

void EnemyManager::tick_spawning(const float& deltaTime)
{
    static float timer = spawnInterval; // Static variable to track time between enemy spawns.

    // If the number of active enemies would exceed the maximum allowed cancel any spawning attempt
    if (activeEnemies.size() >= maxEnemies)
        return;

    // Decrease spawnTimer (Countdown)
    timer -= deltaTime;
    // If the timer did not reach zero, cancel spawn attempt
    if (timer > 0.0f)
        return;

    timer += spawnInterval; // Reset the timer to the configured spawn interval
    spawnEnemy();
}

void EnemyManager::tick_enemies(const float& deltaTime)
{
    const ICollidable::RenderInfo playerRenderInfo = gameInstance.getPlayer()->getRenderInfo();

    for (const auto& pair : activeEnemies)
    {
        if (!pair.second)
            continue;

        pair.second->tick(deltaTime, playerRenderInfo);
        enemyRenderer.setVelocity(pair.first, pair.second->getVelocity());
    }
}

void EnemyManager::tick(const float& deltaTime)
{
    // Kill all pendingKill enemies
    tick_kill(deltaTime);

    // Spawning behavior of Enemies
    tick_spawning(deltaTime);

    // Ticking of each enemy
    tick_enemies(deltaTime);

    // Update the enemy renderer at last
    enemyRenderer.tick(deltaTime);
}

void EnemyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	enemyRenderer.draw(target, states);
}