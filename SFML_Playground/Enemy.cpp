#pragma once

#include "Enemy.h" // Own Header File
#include "GameInstance.h"
#include "EnemyManager.h"
#include "SaveGame.h"

Enemy::Enemy()
	: Entity(EntityType::Enemy),
	healthBar(1.0f),
	collisionBox(this, getPosition(), getSize())
{}

IMovable::RenderInfo Enemy::makeSpawnInfo()
{
	sf::Vector2f position = getNewSpawnPos();
	static constexpr float SIZE_X = 100.0f;
	static constexpr float SIZE_Y = 100.0f;
	float rotation = getLookAtRot(getPosition(), gameInstance().getPlayer()->getPosition());
	static const float START_V_X = 0.0f;
	static const float START_V_Y = 0.0f;
	static const sf::Color COLOR = sf::Color(255, 255, 255, 255);

	IMovable::RenderInfo renderInfo = {
		.pos = position,
		.size = {SIZE_X, SIZE_Y},
		.rot = rotation,
		.velocity = {START_V_X, START_V_Y},
		.color = COLOR,
	};

	if (enemyIndex == 0)
		renderInfo.color = sf::Color::Red;

	return renderInfo;
}

sf::Vector2f Enemy::getNewSpawnPos() const
{
	constexpr float MIN_DISTANCE = 800.0f;
	constexpr float MAX_DISTANCE = 1200.0f;

	// Getting values for generating a position
	sf::Vector2f playerPos = GI_Arena::getInstance().getPlayer()->getPosition();

	// Generating the random position
	float distance = RNG::floatInRange(MIN_DISTANCE, MAX_DISTANCE);
	sf::Vector2f generatedPos = RNG::posInDistanceFrom(playerPos, distance);

	return generatedPos;
}

void Enemy::spawn()
{
	CollisionManager::getInstance().registerEnemy(getCollision());

	setRenderInfo(makeSpawnInfo());
}

void Enemy::tick(const float& deltaTime)
{
	// Calculate movement and apply i
	tick_move(deltaTime);

	if (healthBar.isEmpty())
	{
		constexpr bool diedByPlayer = true;
		healthBar.reset();
		kill_self(diedByPlayer);
	}

	return;
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

void Enemy::kill_self(const bool& bByPlayer = false) const
{
	if (bByPlayer)
	{
		SaveGame::currentData.score = (SaveGame::currentData.score + 1);
		gameInstance().getPlayer()->getFlashlight().addDeathLight(getPosition());

	}
	EnemyManager::getInstance().callDelete(enemyIndex);
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	if (pos == getPosition()) return;
	IMovable::setPosition(pos);
	EnemyManager::getInstance().callUpdate(enemyIndex, InfoType::POSITION);
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
	EnemyManager::getInstance().callUpdate(enemyIndex, InfoType::SIZE);
	collisionBox.setSize(size);
}

void Enemy::setColor(const sf::Color& color)
{
	if (color == getColor()) return;
	IMovable::setColor(color);
	EnemyManager::getInstance().callUpdate(enemyIndex, InfoType::COLOR);
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
	{}

void Enemy::collideWithProjectile(Projectile& projectile)
{
	std::cout << "Got hit with [" << projectile.getDamage() << "] damage" << std::endl;
	hurt(projectile.getDamage());
}