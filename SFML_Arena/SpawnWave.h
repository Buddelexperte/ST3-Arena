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
	unsigned int maxEnemiesPerInterval = 20; // Safe guard to be overridden

	virtual void spawnEnemy();
	virtual void spawnInterval();
	virtual SpawnInformation makeSpawnInfo();

protected:
	virtual sf::Vector2f getNewSpawnPos();
	virtual IMovable::RenderInfo makeSpawnRenderInfo();

	virtual float getEnemyHealth() const;
	virtual float getEnemyDamage() const;

public:
	EnemySpawnWave(float sInterval, unsigned int enemiesPerInterval, unsigned int maxIntervals, unsigned int maxEnemiesPerInterval);
	EnemySpawnWave(float sInterval, unsigned int enemiesPerInterval, unsigned int maxIntervals);
	~EnemySpawnWave() = default;

	void tick_timer(const float& deltaTime)
	{
		intervalTimer.addValue(-deltaTime);
	}

	void forgetNumEnemiesSpawned()
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

	virtual bool isNotEnoughEnemies() const
	{
		bool enoughEnemies = (enemiesSpawned >= maxEnemiesPerInterval);

		bool limitedIntervals = (maxIntervals > 0);
		bool enoughIntervals = (nInterval >= maxIntervals && limitedIntervals);

		if (enoughEnemies || enoughIntervals)
			return false;

		return true;
	}

	virtual void trySpawn()
	{
		if (intervalTimer.isEmpty())
		{
			spawnInterval();
			nInterval++;
			// Reset the timer for the next interval
			intervalTimer.fill_to_max();
			forgetNumEnemiesSpawned();
		}
	}
};