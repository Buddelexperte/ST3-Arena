#pragma once

#include "Enemy.h" // Own Header File
#include "EntityManager.h"
#include "GameInstance.h"
#include "SaveGame.h"

Enemy::Enemy()
	: Entity(EntityType::Enemy),
	collisionBox(this, getPosition(), getSize())
{

}

void Enemy::spawn(const SpawnInformation& spawnInfo)
{
	setRenderInfo(spawnInfo.renderInfo);
	setMaxHealth(spawnInfo.health);
	resetHealth();
	setDamage(spawnInfo.damage);
}

void Enemy::tick(const float& deltaTime)
{
	tick_move(deltaTime);

	if (isDead())
	{
		constexpr bool diedByPlayer = true;
		kill_self(diedByPlayer);
		return;
	}
}

void Enemy::releaseToPool()
{
	GenericPool<Enemy>::instance().release(std::unique_ptr<Enemy>(this));
}

void Enemy::tick_move(const float& deltaTime)
{
	// TODO: PROTOTYPE MOVEMENT LOGIC
	Player* playerRef = gameInstance().getPlayer();
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

void Enemy::spawnDeathParticle()
{
	static constexpr float P_LIFETIME = 1.0f;

	IMovable::RenderInfo expRenderInfo = getRenderInfo();
	expRenderInfo.size = getSize() / 2.0f;
	expRenderInfo.color = sf::Color::Yellow;
	expRenderInfo.velocity = sf::Vector2f(0.0f, 0.0f);

	SpawnInformation spawnInfo = {
		.renderInfo = expRenderInfo,
		.health = P_LIFETIME
	};

	EntityManager::getInstance().spawnEntity<P_Sparkle>(spawnInfo); // Expr value in Health slot
}

void Enemy::kill_self(const bool& bByPlayer = false)
{
	if (bByPlayer)
	{
		SaveGame::currentData.score = (SaveGame::currentData.score + 1);
		spawnDeathParticle();
	}

	Entity::kill_self();
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	if (pos == getPosition()) return;
	Entity::setPosition(pos);
	EntityManager::getInstance().callUpdate(getID(), InfoType::POSITION);
	collisionBox.setPos(pos);
}

void Enemy::addPosition(const sf::Vector2f& delta)
{
	if (shouldZero(delta))
		return;
	Entity::addPosition(delta);
	collisionBox.setPos(collisionBox.getPos() + delta);
}

void Enemy::setSize(const sf::Vector2f& size)
{
	if (size == getSize()) return;
	Entity::setSize(size);
	EntityManager::getInstance().callUpdate(getID(), InfoType::SIZE);
	collisionBox.setSize(size);
}

void Enemy::setColor(const sf::Color& color)
{
	if (color == getColor()) return;
	Entity::setColor(color);
	EntityManager::getInstance().callUpdate(getID(), InfoType::COLOR);
}

void Enemy::setRenderInfo(const RenderInfo& newRenderInfo)
{
	Entity::setRenderInfo(newRenderInfo);
	collisionBox.setPos(newRenderInfo.pos);
	collisionBox.setSize(newRenderInfo.size);
}

void Enemy::onCollision(IHasCollision* other)
{
	other->collideWithEnemy(*this);
}

void Enemy::collideWithPlayer(Player& player)
	{}

void Enemy::collideWithProjectile(Projectile& projectile)
{
	hurt(projectile.getDamage());
}