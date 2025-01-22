#pragma once

#include "GameInstance.h"
#include "EnemyManager.h"
#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
	: renderInfo(), gameInstance(&GI_Arena::getInstance()), manager(&EnemyManager::getInstance())
{}

sf::Vector2f Enemy::getNewSpawnPos() const
{
	// Getting values for generating a position
	sf::Vector2f playerPos = gameInstance->getPlayer()->getPos();

	// Generating the random position
	float distance = random.floatInRange(600.0f, 1000.0f);
	sf::Vector2f generatedPos = random.posInDistance(playerPos, distance);

	// Take velocity into calculation for "movement prediction"
	sf::Vector2f velocity = gameInstance->getPlayer()->getVelocity();
	generatedPos += velocity * 10.0f;

	return generatedPos;
}

void Enemy::spawn()
{
	spawn(getNewSpawnPos());
}

void Enemy::spawn(const sf::Vector2f& pos)
{
	setPosition(pos);

	if (enemyIndex == 0)
		setColor(sf::Color::Red);
}

void Enemy::tick(const float& deltaTime)
{
	renderInfo.velocity = (gameInstance->getPlayer()->getVelocity() * random.floatInRange(0.5f, 1.5f)); 

	renderInfo.pos += renderInfo.velocity * deltaTime;
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	if (pos == renderInfo.pos) return;
	
	renderInfo.pos = pos;
	manager->callUpdate(enemyIndex);
}

void Enemy::setSize(const sf::Vector2f& size)
{
	if (size == renderInfo.size) return;

	renderInfo.size = size;
	manager->callUpdate(enemyIndex);
}

void Enemy::setColor(const sf::Color& color)
{
	if (color == renderInfo.color) return;

	renderInfo.color = color;
	manager->callUpdate(enemyIndex);
}