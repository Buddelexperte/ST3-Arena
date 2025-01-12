#pragma once

#include "GameInstance.h"
#include "EnemyManager.h"
#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
	: velocity(0.0f, 0.0f), gameInstance(&GI_Arena::getInstance())
{
	setPosition(pos);
	setSize(size);
	setColor(color);
}

void Enemy::tick(const float& deltaTime)
{
	velocity = gameInstance->getPlayer()->getVelocity();
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	shape.setPosition(pos);
}

void Enemy::setSize(const sf::Vector2f& size)
{
	shape.setSize(size);
	shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
}

void Enemy::setColor(const sf::Color& color)
{
	shape.setFillColor(color);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
}