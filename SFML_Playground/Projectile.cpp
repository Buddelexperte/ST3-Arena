#pragma once

#include "GameInstance.h"
#include "ProjectileManager.h"
#include "Projectile.h"

// PROJECTILE SPAWNER ----------------------------------------
 
ProjectileSpawner::ProjectileSpawner()
	: 
	gameInstance(&GI_Arena::getInstance()),
	manager(&ProjectileManager::getInstance())
	{}

void ProjectileSpawner::spawnProjectile()
{
	RenderInfo projectileInfo = {
		projectileInfo.pos = getPosition(),
		projectileInfo.size = baseInfo.size,
		projectileInfo.rot = getRotation() + baseInfo.rot,
		projectileInfo.velocity = dirFromRot(getRotation()) * baseInfo.velocity * 100.0f,
		projectileInfo.color = baseInfo.color
	};
	
	manager->shootProjectile(projectileInfo);
}

// PROJECTILE OBJECT ----------------------------------------

void Projectile::onCollision(ICollidable* other)
{
	GI_Arena::getInstance().getPlayer()->getInventory().triggerPerks(PerkTrigger::OnWeaponHit);
}