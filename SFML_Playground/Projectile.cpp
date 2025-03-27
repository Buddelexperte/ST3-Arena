#pragma once

#include "GameInstance.h"
#include "Projectile.h"
#include "EntityManager.h"

// PROJECTILE SPAWNER ----------------------------------------
 
ProjectileSpawner::ProjectileSpawner(const RenderInfo& baseInfo, const float& damage)
	:
	baseInfo(baseInfo),
	damage(damage)
{

}

void ProjectileSpawner::shoot()
{
	RenderInfo projectileInfo = {
		projectileInfo.pos = getPosition() + baseInfo.pos,
		projectileInfo.size = baseInfo.size,
		projectileInfo.rot = getRotation() + baseInfo.rot,
		projectileInfo.velocity = dirFromRot(getRotation()) * baseInfo.velocity,
		projectileInfo.color = baseInfo.color
	};
	
	SpawnInformation spawnInfo = {
		.renderInfo = projectileInfo,
		.damage = getDamage()
	};

	EntityManager::getInstance().spawnEntity<Projectile>(spawnInfo);
}

// PROJECTILE OBJECT ----------------------------------------

Projectile::Projectile()
	: Projectile(RenderInfo({0.0f, 0.0f}, {100.0f, 100.0f}))
{}

Projectile::Projectile(const RenderInfo& initRenderInfo)
	: Entity(EntityType::Projectile), 
	lifetime(MAX_LIFETIME),
	collisionBox(this, initRenderInfo.pos, initRenderInfo.size)
{
	setRenderInfo(initRenderInfo);
}

void Projectile::tick_move(const float& deltaTime)
{
	sf::Vector2f delta = getVelocity() * deltaTime;
	addPosition(delta);
}

void Projectile::tick_lifetime(const float& deltaTime)
{
	lifetime.addValue(deltaTime);

	if (lifetime.isEmpty())
	{
		kill_self();
	}
}

void Projectile::spawn(const SpawnInformation& spawnInfo)
{
	lifetime.reset();
	setRenderInfo(spawnInfo.renderInfo);
	setDamage(spawnInfo.damage);
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