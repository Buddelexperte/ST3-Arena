#pragma once

#include "GameInstance.h"
#include "ProjectileManager.h"

// PROJECTILE SPAWNER ----------------------------------------
 
ProjectileSpawner::ProjectileSpawner(const RenderInfo& baseInfo)
	: 
	gameInstance(&GI_Arena::getInstance()),
	manager(&ProjectileManager::getInstance()),
	baseInfo(baseInfo)
	{}

void ProjectileSpawner::spawnProjectile()
{
	RenderInfo projectileInfo = {
		projectileInfo.pos = getPosition() + baseInfo.pos,
		projectileInfo.size = baseInfo.size,
		projectileInfo.rot = getRotation() + baseInfo.rot,
		projectileInfo.velocity = dirFromRot(getRotation()) * baseInfo.velocity,
		projectileInfo.color = baseInfo.color
	};
	
	manager->shootProjectile(projectileInfo);
}

// PROJECTILE OBJECT ----------------------------------------

Projectile::Projectile()
	: Projectile(RenderInfo({0.0f, 0.0f}, {100.0f, 100.0f}))
{}

Projectile::Projectile(const RenderInfo& initRenderInfo)
	: collisionBox(this, initRenderInfo.pos, initRenderInfo.size)
{
	setRenderInfo(initRenderInfo);
}

Projectile::~Projectile()
{
	CollisionManager::getInstance().unregisterCollidable(getCollision()->getCollisionID());
}

void Projectile::kill_self() const
{
	ProjectileManager::getInstance().callDelete(projectileID);
}

void Projectile::tick_move(const float& deltaTime)
{
	sf::Vector2f delta = getVelocity() * deltaTime;
	addPosition(delta);
}

void Projectile::tick_lifetime(const float& deltaTime)
{
	lifetimeLeft -= deltaTime;

	if (lifetimeLeft < SMALLEST_PRECISION)
		kill_self();
}

void Projectile::onCollision(IHasCollision* other)
{
	other->collideWithProjectile(*this);
}

void Projectile::collideWithEnemy(Enemy& enemy)
{
	GI_Arena::getInstance().getPlayer()->getInventory().triggerPerks(PerkTrigger::OnEnemyHit);
	kill_self();
}