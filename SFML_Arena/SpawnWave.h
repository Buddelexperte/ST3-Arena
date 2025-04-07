#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

class EnemySpawnWave
{
private:
	ValueBar intervalTimer; // Timer to track the spawn interval
	unsigned int enemiesPerInterval; // Number of enemies to spawn per interval
	unsigned int maxIntervals; // Number of intervals to spawn enemies

	unsigned int nInterval = 0;
	unsigned int enemiesSpawned = 0; // Number of enemies spawned in the current interval


	virtual sf::Vector2f getNewSpawnPos();
	virtual IMovable::RenderInfo makeSpawnRenderInfo();
	virtual SpawnInformation makeSpawnInfo();

	virtual void spawnEnemy();
	virtual void spawnInterval();
public:
	EnemySpawnWave(float sInterval, unsigned int enemiesPerInterval, unsigned int maxIntervals)
		: intervalTimer(sInterval),
		enemiesPerInterval(enemiesPerInterval),
		maxIntervals(maxIntervals)
	{

	}
	~EnemySpawnWave() = default;

	void tick_timer(const float& deltaTime)
	{
		intervalTimer.addValue(-deltaTime);
	}

	void resetEnemiesSpawned()
	{
		enemiesSpawned = 0;
	}

	void incrEnemiesSpawned()
	{
		enemiesSpawned++;
	}

	unsigned int getEnemiesSpawned() const
	{
		return enemiesSpawned;
	}

	virtual void trySpawn()
	{
		if (nInterval >= maxIntervals && maxIntervals > 0)
			return;

		if (intervalTimer.isEmpty())
		{
			spawnInterval();
			nInterval++;
			// Reset the timer for the next interval
			intervalTimer.reset();
		}
	}
};