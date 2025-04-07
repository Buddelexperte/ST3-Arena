#pragma once

#include "EnemySpawner.h" // Own header file
#include "EntityManager.h"
#include "GameInstance.h"

EnemySpawner::EnemySpawner()
{
	// TODO: Add dynamic Wave progression
	spawnWave = std::make_unique<SW_Stage1>();
}

void EnemySpawner::spawnEnemy(const SpawnInformation& spawnInfo)
{
}

void EnemySpawner::tick_timer(const float& deltaTime)
{
	if (spawnWave == nullptr)
		return;

	spawnWave->tick_timer(deltaTime);
}

bool EnemySpawner::canSpawn() const
{
	return EntityManager::getInstance().getNumEnemies() < MAX_ENEMIES;
}

void EnemySpawner::trySpawnEnemy()
{
    if (spawnWave == nullptr)
        return;

	spawnWave->trySpawn();
}

void EnemySpawner::setSpawnWave(std::unique_ptr<EnemySpawnWave> newSW)
{
	if (newSW)
	{
		spawnWave = std::move(newSW);
		return;
	}

	spawnWave = std::make_unique<SW_Default>();
}
