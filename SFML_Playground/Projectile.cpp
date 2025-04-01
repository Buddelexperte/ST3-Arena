#pragma once

#include "GameInstance.h"
#include "Projectile.h"
#include "EntityManager.h"

#include "AllEntities.h"

// PROJECTILE SPAWNER ----------------------------------------
 
void ProjectileSpawner::spawnParticle(const SpawnInformation& spawnInfo)
{
	static constexpr float P_LIFETIME = 0.8f;

	static const sf::Vector2f P_VELOCITY = sf::Vector2f(0.0f, 0.0f);
	static const sf::Vector2f P_SIZE = sf::Vector2f(50.0f, 50.0f);
	static const sf::Color P_COLOR = sf::Color(100, 100, 100, 255);

	IMovable::RenderInfo particleRenderInfo = {
		.pos = spawnInfo.renderInfo.pos,
		.size = P_SIZE,
		.rot = spawnInfo.renderInfo.rot,
		.velocity = P_VELOCITY,
		.color = P_COLOR
	};

	SpawnInformation particleInfo = {
		.renderInfo = particleRenderInfo,
		.health = P_LIFETIME
	};

	EntityManager::getInstance().spawnEntity<P_Sparkle>(particleInfo); // Expr value in Health slot
}

ProjectileSpawner::ProjectileSpawner(const RenderInfo& baseInfo, const float& damage)
	:
	baseInfo(baseInfo),
	damage(damage)
{

}

void ProjectileSpawner::shoot(const SpawnInformation& spawnInfo)
{
	EntityManager::getInstance().spawnEntity<Projectile>(spawnInfo);

	spawnParticle(spawnInfo);
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