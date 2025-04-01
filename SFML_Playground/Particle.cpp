#pragma once

#include "Particle.h" // Own header
#include "GameInstance.h"

Particle::Particle()
	: Entity(EntityType::PARTICLE),
	IHasLifetime(0.0f),
	collisionBox(this, getPosition(), getSize())
{

}

void Particle::spawn(const SpawnInformation& spawnInfo)
{
	setRenderInfo(spawnInfo.renderInfo);
	resetLifetime(spawnInfo.health);
}

void Particle::tick(const float& deltaTime)
{
	tick_lifetime(deltaTime);

	tick_move(deltaTime);
}

void Particle::releaseToPool()
{
	GenericPool<Particle>::instance().release(std::unique_ptr<Particle>(this));
}