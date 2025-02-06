#pragma once

#include "GameInstance.h"
#include "EnemyManager.h"
#include "Enemy.h"

Enemy::Enemy()
	: renderInfo(), gameInstance(&GI_Arena::getInstance()), manager(&EnemyManager::getInstance())
{}

sf::Vector2f Enemy::getNewSpawnPos() const
{
	// Getting values for generating a position
	sf::Vector2f playerPos = gameInstance->getPlayer()->getPos();

	// Generating the random position
	float distance = random.floatInRange(200.0f, 1000.0f);
	sf::Vector2f generatedPos = random.posInDistance(playerPos, distance);

	// TODO: Take velocity into calculation for "movement prediction"
	sf::Vector2f velocity = gameInstance->getPlayer()->getVelocity();

	return generatedPos;
}

void Enemy::spawn()
{
	setPosition(getNewSpawnPos());

	if (enemyIndex == 0)
		setColor(sf::Color::Red);
}

// Fast circle-circle collision check
bool Enemy::isColliding(const sf::Vector2f& playerPos, const float playerRadius) const
{
	float dx = playerPos.x - renderInfo.pos.x;
	float dy = playerPos.y - renderInfo.pos.y;
	float distanceSquared = dx * dx + dy * dy;
	float radiusSum = playerRadius + collisionRadius;
	return distanceSquared <= (radiusSum * radiusSum);
}

void Enemy::tick_move(const float& deltaTime)
{
	// Get the player's position and subtract own position
	const Player* playerRef = gameInstance->getPlayer();
	const sf::Vector2f playerPos = playerRef->getPos();
	const float playerRadius = playerRef->getCollisionRadius();
	sf::Vector2f distance = playerPos - renderInfo.pos;
	zeroPrecision(distance);

	// Get Velocity by direction * (speed / normalized direction)
	renderInfo.velocity = distance * (speed / (std::abs(distance.x) + std::abs(distance.y)));

	// Update the position based on velocity and scale by deltaTime
	renderInfo.pos += renderInfo.velocity * deltaTime;

	// COLLISION
	if (isColliding(playerPos, playerRadius))
	{
		die();
	}
}

void Enemy::tick(const float& deltaTime)
{
	tick_move(deltaTime);

	return;
}

void Enemy::die()
{
	manager->callDelete(enemyIndex);
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	if (pos == renderInfo.pos) return;
	
	renderInfo.pos = pos;
	manager->callUpdate(enemyIndex, InfoType::POSITION);
}

void Enemy::setSize(const sf::Vector2f& size)
{
	if (size == renderInfo.size) return;

	renderInfo.size = size;
	manager->callUpdate(enemyIndex, InfoType::SIZE);
}

void Enemy::setColor(const sf::Color& color)
{
	if (color == renderInfo.color) return;

	renderInfo.color = color;
	manager->callUpdate(enemyIndex, InfoType::COLOR);
}

void Enemy::setRenderInfo(const RenderInfo& renderInfo)
{
	this->renderInfo = renderInfo;
}