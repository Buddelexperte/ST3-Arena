#pragma once
#include <iostream>

#include "EnemyManager.h"

EnemyManager::EnemyManager()
	: enemyPool(), enemyRenderer(), gameInstance(GI_Arena::getInstance())
{}

int EnemyManager::getNumActiveEnemies() const
{
	return activeEnemies.size();
}

void EnemyManager::spawnEnemy()
{
	spawnEnemy(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(100.0f, 100.0f), sf::Color::White);
}

void EnemyManager::spawnEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
{
    // Retrieve an enemy instance from the pool
    std::unique_ptr<Enemy> newEnemy = enemyPool.get(pos, size, color);

    // Extract render information and pass it to the renderer
    const Enemy::EnemyRenderInfo& renderInfo = newEnemy->getRenderInfo();
    enemyRenderer.addEnemy(renderInfo);

    // Set the enemy's index and add it to the activeEnemies vector
    size_t enemyIndex = getNumActiveEnemies();
    newEnemy->setIndex(enemyIndex);

    // Actually spawn the enemy properly and update it's attributes accordingly
    activeEnemies.push_back(std::move(newEnemy));
    activeEnemies.back()->spawn();
}

void EnemyManager::deleteEnemy(const size_t& index)
{
	enemyPool.release(std::move(activeEnemies[index]));

	enemyRenderer.removeEnemy(index);
}

void EnemyManager::callUpdate(const size_t& index)
{
	enemyRenderer.setPosition(index, activeEnemies[index]->getPosition());
	enemyRenderer.setSize(index, activeEnemies[index]->getSize());
	enemyRenderer.setColor(index, activeEnemies[index]->getColor());
}

void EnemyManager::tick_spawning(const float& deltaTime)
{
    // TODO: Move spawning logic to a dedicated class
    static float timer = spawnInterval; // Static variable to track time between enemy spawns.

    // If the number of active enemies would exceed the maximum allowed cancel any spawning attempt
    if (getNumActiveEnemies() >= maxEnemies)
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
    // Update the renderer with the velocity of each active enemy, for clean movement
    for (size_t i = 0; i < activeEnemies.size(); i++)
    {
        activeEnemies[i]->tick(deltaTime);
        enemyRenderer.setVelocity(i, activeEnemies[i]->getVelocity());
    }
}

void EnemyManager::tick(const float& deltaTime)
{
    if (gameInstance.getIsPaused())
        return;

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