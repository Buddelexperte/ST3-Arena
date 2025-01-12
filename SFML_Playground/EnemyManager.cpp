#pragma once
#include "EnemyManager.h"

EnemyManager::EnemyManager()
	: enemyPool(), enemyRenderer()
{}

void EnemyManager::spawnEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
{
	activeEnemies.push_back(enemyPool.get(pos, size, color));

	enemyRenderer.addEnemy(pos, size, color);

	// Testing the velocity in the EnemyRenderer, DEV
	enemyRenderer.setVelocity(activeEnemies.size() - 1, { 350.0f, 0.0f });
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