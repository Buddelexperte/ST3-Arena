#pragma once
#include <iostream>
#include "EnemyManager.h"

EnemyManager::EnemyManager()
	: enemyPool(), enemyRenderer()
{}

int EnemyManager::getActiveEnemies() const
{
	return activeEnemies.size();
}

void EnemyManager::spawnEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
{
	activeEnemies.push_back(enemyPool.get(pos, size, color));

	enemyRenderer.addEnemy(pos, size, color);
}

void EnemyManager::deleteEnemy(const size_t& index)
{
	//enemyPool.release(std::move(activeEnemies[index]));

	enemyRenderer.removeEnemy(index);
}

void EnemyManager::tick(const float& deltaTime)
{
	// Tick all enemies first
	for (std::unique_ptr<Enemy>& enemy : activeEnemies)
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