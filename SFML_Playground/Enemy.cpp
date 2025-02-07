#pragma once

#include "GameInstance.h"
#include "EnemyManager.h"
#include "Enemy.h"

Enemy::Enemy()
	: renderInfo(), gameInstance(&GI_Arena::getInstance()), manager(&EnemyManager::getInstance())
{
	collisionRect = {
		renderInfo.pos.x - (renderInfo.size.x * 0.5f),  // Top-left X
		renderInfo.pos.y - (renderInfo.size.y * 0.5f),  // Top-left Y
		renderInfo.size.x,                              // Width
		renderInfo.size.y                               // Height
	};

	setPlayer(gameInstance->getPlayer());
}

void Enemy::setPlayer(Player* playerPtr)
{
	playerRef = playerPtr;
}

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

	if (!(enemyIndex % 3))
		setColor(sf::Color::Red);
}

// Fast circle-circle collision check
bool Enemy::isCollidingWith(const sf::FloatRect& playerCollision) const
{
	return collisionRect.intersects(playerCollision);
}

void Enemy::tick_move(const float& deltaTime)
{
	// TODO: PROTOTYPE MOVEMENT LOGIC
	const sf::Vector2f playerPos = playerRef->getPos();
	sf::Vector2f distance = playerPos - renderInfo.pos;
	zeroPrecision(distance);

	if (distance != sf::Vector2f(0.0f, 0.0f))
	{
		// Get Velocity by direction * (speed / normalized direction)
		renderInfo.velocity = distance * (speed / (std::abs(distance.x) + std::abs(distance.y)));

		// Update the position based on velocity and scale by deltaTime
		const sf::Vector2f offset = renderInfo.velocity * deltaTime;
		renderInfo.pos += offset;
	
		// COLLISION
		collisionRect.left += offset.x;
		collisionRect.top += offset.y;
	}

	const sf::FloatRect playerCollisionRect = playerRef->getCollisionRect();
	if (isCollidingWith(playerCollisionRect))
	{
		// TODO: PROTOTYPE COLLISION LOGIC
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

void Enemy::setPosition(const sf::Vector2f& newPos)
{
	if (newPos == renderInfo.pos) return;

	renderInfo.pos = newPos;
	manager->callUpdate(enemyIndex, InfoType::POSITION);

	// Collision rectangle update: subtract half size for both X and Y
	collisionRect.left = newPos.x - (renderInfo.size.x / 2.0f);
	collisionRect.top = newPos.y - (renderInfo.size.y / 2.0f);
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