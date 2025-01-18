#pragma once
#include <iostream>
#include "EnemyManager.h"

EnemyManager::EnemyManager()
	: enemyPool(), enemyRenderer()
{}

int EnemyManager::getNumActiveEnemies() const
{
	return activeEnemies.size();
}

void EnemyManager::spawnEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
{
	// Retrieve an enemy instance from the pool
	std::unique_ptr<Enemy> newEnemy = enemyPool.get(pos, size, color);

	// Extract render information and pass it to the renderer
	const Enemy::EnemyRenderInfo& renderInfo = newEnemy->getRenderInfo();
	enemyRenderer.addEnemy(renderInfo.pos, renderInfo.size, renderInfo.velocity, renderInfo.color);

	// Set the enemy's index and add it to the active enemies list
	newEnemy->setIndex(getNumActiveEnemies());
	activeEnemies.push_back(std::move(newEnemy));
	activeEnemies.back()->spawn(pos);
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

void EnemyManager::tick(const float& deltaTime)
{
	// Tick all enemies first
	for (const std::unique_ptr<Enemy>& enemy : activeEnemies)
	{
		enemy->tick(deltaTime);
	}

	for (size_t i = 0; i < activeEnemies.size(); i++)
	{
		enemyRenderer.setVelocity(i, activeEnemies[i]->getVelocity());
	}

	// Update the Renderer at last
	enemyRenderer.tick(deltaTime);
}

void EnemyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	enemyRenderer.draw(target, states);
}