#pragma once

#include "GameInstance.h"
#include "Projectile.h"
#include "EntityManager.h"

#include "AllEntities.h"

// PROJECTILE SPAWNER ----------------------------------------
 
void ProjectileSpawner::spawnParticle(const SpawnInformation& spawnInfo)
{
	//TODO: Muzzle Flash
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

void Projectile::spawn( SpawnInformation spawnInfo)
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
	PerkTriggerInfo triggerInfo(PerkTrigger::OnEnemyGotHit, getPosition(), this);
	GI_Arena::getInstance().getPlayer()->getInventory().triggerPerks(triggerInfo);
	kill_self();
}