#pragma once

#include "GameInstance.h"
#include "EnemyManager.h"
#include "Enemy.h"

Enemy::Enemy()
	: 
	gameInstance(&GI_Arena::getInstance()), 
	manager(&EnemyManager::getInstance()),
	collisionBox(getPosition(), getSize())
{
	setPlayer(gameInstance->getPlayer());
}

void Enemy::setPlayer(Player* playerPtr)
{
	playerRef = playerPtr;
}

sf::Vector2f Enemy::getNewSpawnPos() const
{
	// Getting values for generating a position
	sf::Vector2f playerPos = gameInstance->getPlayer()->getPosition();

	// Generating the random position
	float distance = random.floatInRange(200.0f, 1000.0f);
	sf::Vector2f generatedPos = random.posInDistanceFrom(playerPos, distance);

	return generatedPos;
}

void Enemy::spawn()
{
	setPosition(getNewSpawnPos());

	if (!(enemyIndex % 3))
		setColor(sf::Color::Red);
}

void Enemy::tick(const float& deltaTime, const RenderInfo& playerRenderInfo)
{
	// Calculate movement and apply i
	tick_move(deltaTime, playerRenderInfo);

	// Check for collision updates
	tick_collision(deltaTime);

	return;
}

void Enemy::tick_move(const float& deltaTime, const RenderInfo& playerRenderInfo)
{
	// TODO: PROTOTYPE MOVEMENT LOGIC
	sf::Vector2f distance = playerRenderInfo.pos - getPosition();
	zeroPrecision(distance);

	// On target contact, no need for calculations, just set speed to zero
	if (distance == sf::Vector2f(0.0f, 0.0f))
	{
		setVelocity({ 0.0f, 0.0f });
		return;
	}

	// Get Velocity by direction * (speed / normalized direction)
	const float norm = std::abs(distance.x) + std::abs(distance.y);
	// Calculate velocity: scale the direction vector by speed/normalized length.
	setVelocity(distance * (speed / norm));

	// Update the position based on velocity and scale by deltaTime
	const sf::Vector2f offset = getVelocity() * deltaTime;
	addPosition(offset);
}

void Enemy::tick_collision(const float& deltaTime)
{
	const sf::FloatRect playerCollisionRect = playerRef->getCollisionBounds();
	if (isColliding(playerCollisionRect))
	{
		// TODO: PROTOTYPE COLLISION LOGIC
		die();
	}
}

// TODO: Implement onCollision instead of constant checking in the loop
void Enemy::onCollision(ICollidable* other)
{

}

void Enemy::die()
{
	manager->callDelete(enemyIndex);
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	if (pos == getPosition()) return;
	IMovable::setPosition(pos);
	manager->callUpdate(enemyIndex, InfoType::POSITION);
	collisionBox.setPos(pos);
}

void Enemy::addPosition(const sf::Vector2f& delta)
{
	IMovable::addPosition(delta);
	collisionBox.setPos(collisionBox.getPos() + delta);
}

void Enemy::setSize(const sf::Vector2f& size)
{
	if (size == getSize()) return;
	IMovable::setSize(size);
	manager->callUpdate(enemyIndex, InfoType::SIZE);
	collisionBox.setSize(size);
}

void Enemy::setColor(const sf::Color& color)
{
	if (color == getColor()) return;
	IMovable::setColor(color);
	manager->callUpdate(enemyIndex, InfoType::COLOR);
}

void Enemy::setRenderInfo(const RenderInfo& newRenderInfo)
{
	IMovable::setRenderInfo(newRenderInfo);
	collisionBox.setPos(getPosition());
	collisionBox.setSize(getSize());
}