#pragma once

#include "GameInstance.h"
#include "Projectile.h"
#include "EntityManager.h"

#include "AllEntities.h"

// PROJECTILE SPAWNER ----------------------------------------
 
ProjectileSpawner::ProjectileSpawner(const RenderInfo& baseInfo, const float& damage)
	:
	baseInfo(baseInfo),
	damage(damage)
{

}

void ProjectileSpawner::shoot(const SpawnInformation& spawnInfo)
{
	EntityManager::getInstance().spawnEntity<Projectile>(spawnInfo);

	return; // TODO: Add good looking fire particles

	static constexpr float P_LIFETIME = 0.8f;

	IMovable::RenderInfo particleRenderInfo = getRenderInfo();
	particleRenderInfo.pos = spawnInfo.renderInfo.pos;
	particleRenderInfo.size = sf::Vector2f(50.0f, 50.0f);
	particleRenderInfo.color = sf::Color(100, 100, 100, 255);
	particleRenderInfo.velocity = sf::Vector2f(0.0f, 0.0f);

	SpawnInformation particleInfo = {
		.renderInfo = particleRenderInfo,
		.health = P_LIFETIME
	};

	EntityManager::getInstance().spawnEntity<P_Sparkle>(particleInfo); // Expr value in Health slot
}

// PROJECTILE OBJECT ----------------------------------------

Projectile::Projectile()
	: Projectile(RenderInfo({0.0f, 0.0f}, {100.0f, 100.0f}))
{}

Projectile::Projectile(const RenderInfo& initRenderInfo)
	: Entity(EntityType::Projectile),
	IHasLifetime(MAX_LIFETIME),
	collisionBox(this, initRenderInfo.pos, initRenderInfo.size)
{
	setRenderInfo(initRenderInfo);
}

void Projectile::tick_move(const float& deltaTime)
{
	sf::Vector2f delta = getVelocity() * deltaTime;
	addPosition(delta);
}

void Projectile::spawn(const SpawnInformation& spawnInfo)
{
	setRenderInfo(spawnInfo.renderInfo);
	setDamage(spawnInfo.damage);
	resetLifetime(MAX_LIFETIME);
}

void Projectile::tick(const float& deltaTime)
{
	tick_move(deltaTime);

	tick_lifetime(deltaTime);
}

void Projectile::releaseToPool()
{
	GenericPool<Projectile>::instance().release(std::unique_ptr<Projectile>(this));
}

void Projectile::onCollision(IHasCollision* other)
{
	other->collideWithProjectile(*this);
}

void Projectile::collideWithEnemy(Enemy& enemy)
{
	PerkTriggerInfo triggerInfo(PerkTrigger::OnEnemyHit, getPosition(), this);
	GI_Arena::getInstance().getPlayer()->getInventory().triggerPerks(triggerInfo);
	kill_self();
}