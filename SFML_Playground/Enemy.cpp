#pragma once

#include "GameInstance.h"
#include "EnemyManager.h"
#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
	: velocity(0.0f, 0.0f), gameInstance(&GI_Arena::getInstance()), manager(&EnemyManager::getInstance())
{
	shape.setPosition(pos);
	position = pos;
	shape.setSize(size);
	shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
	shape.setFillColor(color);
}

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
	velocity = (gameInstance->getPlayer()->getVelocity() * random.floatInRange(0.2f, 1.2f)); 

	position += velocity * deltaTime;
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	if (pos == shape.getPosition()) return;
	
	shape.setPosition(pos);
	position = pos;
	manager->callUpdate(enemyIndex);
}

void Enemy::setSize(const sf::Vector2f& size)
{
	if (size == shape.getSize()) return;

	shape.setSize(size);
	shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
	manager->callUpdate(enemyIndex);
}

void Enemy::setColor(const sf::Color& color)
{
	//if (color == shape.getFillColor()) return;

	shape.setFillColor(color);
	manager->callUpdate(enemyIndex);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	manager->callUpdate(enemyIndex);
}