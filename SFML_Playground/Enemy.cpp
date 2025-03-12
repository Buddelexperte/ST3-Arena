#pragma once

#include "Enemy.h" // Own Header File
#include "GameInstance.h"
#include "EnemyManager.h"

Enemy::Enemy()
	: 
	gameInstance(&GI_Arena::getInstance()),
	manager(&EnemyManager::getInstance()),
	healthBar(0.1f),
	collisionBox(this, getPosition(), getSize())
{
	setPlayer(gameInstance->getPlayer());
}

Enemy::~Enemy()
{
	CollisionManager::getInstance().unregisterCollidable(getCollision()->getCollisionID());
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
	CollisionManager::getInstance().registerEnemy(getCollision());
	setPosition(getNewSpawnPos());

	if (enemyIndex == 0)
		setColor(sf::Color::Red);
}

void Enemy::tick(const float& deltaTime)
{
	// Calculate movement and apply i
	tick_move(deltaTime);

	if (healthBar.isEmpty())
	{
		healthBar.reset();
		kill_self();
	}

	return;
}

void Enemy::tick_move(const float& deltaTime)
{
	// TODO: PROTOTYPE MOVEMENT LOGIC
	sf::Vector2f playerPos = playerRef->getPosition();
	sf::Vector2f distance = playerPos - getPosition();

	// On target contact, no need for calculations, just set speed to zero and stop calculations
	if (shouldZero(distance))
	{
		setVelocity({ 0.0f, 0.0f });
		return;
	}

	// Get Velocity by direction * (speed / normalized direction)
	float norm = std::abs(distance.x) + std::abs(distance.y);
	// Calculate velocity: scale the direction vector by speed/normalized length
	sf::Vector2f targetVelo = distance * (WALK_SPEED / norm);
	sf::Vector2f newVelo = lerp(getVelocity(), targetVelo, LERP_SMOOTHNESS);
	sf::Vector2f offset = newVelo * deltaTime;

	addPosition(offset);
	setVelocity(newVelo);

	// Rotation
	
	const sf::Vector2f pos = getPosition();
	float rotation = getRotation();
	float targetRot = getLookAtRot(playerPos, pos);

	if (!(shouldZero(rotation - targetRot)))
	{
		float newRot = lerp(rotation, targetRot, ROT_LERP);
		setRotation(newRot);
	}
}

void Enemy::kill_self()
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
	if (shouldZero(delta)) return;
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
	collisionBox.setPos(newRenderInfo.pos);
	collisionBox.setSize(newRenderInfo.size);
}

// TODO: Implement onCollision instead of constant checking in the loop
void Enemy::onCollision(IHasCollision* other)
{
	other->collideWithEnemy(*this);
}

void Enemy::collideWithPlayer(Player& player)
{
	kill_self();
}

void Enemy::collideWithProjectile(Projectile& projectile)
{
	hurt(projectile.getDamage());
}