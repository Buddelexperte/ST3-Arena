#pragma once

#include "Enemy.h" // Own header file
#include "AllEnemyWaves.h" // All enemy waves

class EnemySpawner
{
private:
	static constexpr float SPAWN_INTERVAL = SMALLEST_PRECISION;
	static constexpr unsigned int MAX_ENEMIES = 20; // TODO: Temporary safe guard, replace with actual spawning logic later


	void spawnEnemy(const SpawnInformation& spawnInfo);


	std::unique_ptr<EnemySpawnWave> spawnWave = nullptr;

public:
	EnemySpawner();
	~EnemySpawner() = default;

	void tick_timer(const float& deltaTime);

	bool canSpawn() const;

	void trySpawnEnemy();
};