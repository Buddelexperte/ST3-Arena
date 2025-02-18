#pragma once

#include "GameInstance.h"
#include "EnemyManager.h"
#include "Enemy.h"

Enemy::Enemy()
	: renderInfo(), 
	gameInstance(&GI_Arena::getInstance()), 
	manager(&EnemyManager::getInstance()),
	collisionBox(renderInfo.pos, renderInfo.size)
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

void Enemy::tick_move(const float& deltaTime)
{
	// TODO: PROTOTYPE MOVEMENT LOGIC
	const sf::Vector2f playerPos = playerRef->getPos();
	sf::Vector2f distance = playerPos - renderInfo.pos;
	zeroPrecision(distance);

	// On target contact, no need for calculations, just set speed to zero
	if (distance == sf::Vector2f(0.0f, 0.0f))
	{
		renderInfo.velocity = { 0.0f, 0.0f };
		return;
	}

	// Get Velocity by direction * (speed / normalized direction)
	renderInfo.velocity = distance * (speed / (std::abs(distance.x) + std::abs(distance.y)));

	// Update the position based on velocity and scale by deltaTime
	const sf::Vector2f offset = renderInfo.velocity * deltaTime;
	renderInfo.pos += offset;
	// COLLISION
	collisionBox.setPos(collisionBox.getPos() + offset);
}

void Enemy::tick_collision(const float& deltaTime)
{
	const sf::FloatRect playerCollisionRect = playerRef->getCollisionBounds();
	std::cout << "PLAYER COLLISION POS: " << playerCollisionRect.getPosition().x << " x " << playerCollisionRect.getPosition().y << " y" << std::endl;
	std::cout << "PLAYER COLLISION SIZE: " << playerCollisionRect.getSize().x << " x " << playerCollisionRect.getSize().y << " y" << std::endl;
	std::cout << "ENEMY COLLISION POS: " << collisionBox.getPos().x << " x " << collisionBox.getPos().y << " y" << std::endl;
	std::cout << "ENEMY COLLISION SIZE: " << collisionBox.getSize().x << " x " << collisionBox.getSize().y << " y" << std::endl;
	if (isColliding(playerCollisionRect))
	{
		std::cout << "COLLISION" << std::endl;
		// TODO: PROTOTYPE COLLISION LOGIC
		die();
	}
}

bool Enemy::isColliding(const sf::FloatRect& otherBound) const
{
	return collisionBox.isColliding(otherBound);
}

bool Enemy::isColliding(const sf::Vector2f& otherPos) const
{
	return collisionBox.isColliding(otherPos);
}

void Enemy::onCollision(ICollidable* other)
{

}

void Enemy::tick(const float& deltaTime)
{

	tick_move(deltaTime);

	tick_collision(deltaTime);

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
	collisionBox.setPos(newPos);
}


void Enemy::setSize(const sf::Vector2f& size)
{
	if (size == renderInfo.size) return;

	renderInfo.size = size;
	manager->callUpdate(enemyIndex, InfoType::SIZE);

	collisionBox.setSize(size);
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
	collisionBox.setPos(renderInfo.pos);
	collisionBox.setSize(renderInfo.size);
}