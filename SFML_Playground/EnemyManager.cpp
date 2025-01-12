#pragma once
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
	enemyPool.release(std::move(activeEnemies[index]));

	enemyRenderer.removeEnemy(index);
}

void EnemyManager::tick(const float& deltaTime)
{
	enemyRenderer.tick(deltaTime);
}

void EnemyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(enemyRenderer, states);
}