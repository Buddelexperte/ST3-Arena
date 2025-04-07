#pragma once 

#include "SpawnWave.h" // Own header file
#include "EntityManager.h"
#include "GameInstance.h"
#include "RandomNumbers.h"

sf::Vector2f EnemySpawnWave::getNewSpawnPos()
{
    constexpr float MIN_DISTANCE = 400.0f; // 800
    constexpr float MAX_DISTANCE = 1200.0f; // 1200

    // Getting values for generating a position
    sf::Vector2f playerPos = gameInstance().getPlayer()->getPosition();

    // Generating the random position
    float distance = RNG::floatInRange(MIN_DISTANCE, MAX_DISTANCE);
    sf::Vector2f generatedPos = RNG::posInDistanceFrom(playerPos, distance);

    return generatedPos;
}

IMovable::RenderInfo EnemySpawnWave::makeSpawnRenderInfo()
{
    const sf::Vector2f playerPos = gameInstance().getPlayer()->getPosition();

    sf::Vector2f position = getNewSpawnPos();
    static constexpr float SIZE_X = 100.0f;
    static constexpr float SIZE_Y = 100.0f;
    float rotation = getLookAtRot(playerPos, position);
    static const float START_V_X = 0.0f;
    static const float START_V_Y = 0.0f;
    static const sf::Color COLOR = sf::Color(255, 255, 255, 255);

    IMovable::RenderInfo renderInfo = {
        .pos = position,
        .size = {SIZE_X, SIZE_Y},
        .rot = rotation,
        .velocity = {START_V_X, START_V_Y},
        .color = COLOR,
    };

    return renderInfo;
}

SpawnInformation EnemySpawnWave::makeSpawnInfo()
{
    // TODO: Temporary values, replace with spawning logic
    static constexpr float ENEMY_HEALTH = 1.0f;
    static constexpr float ENEMY_DAMAGE = 0.1f;

    return SpawnInformation(makeSpawnRenderInfo(), ENEMY_HEALTH, ENEMY_DAMAGE);
}

void EnemySpawnWave::spawnEnemy()
{
    SpawnInformation spawnInfo = makeSpawnInfo();
    EntityManager::getInstance().spawnEntity<Enemy>(spawnInfo);
}

void EnemySpawnWave::spawnInterval()
{
	for (enemiesSpawned = 0; enemiesSpawned < enemiesPerInterval; enemiesSpawned++)
	{
        spawnEnemy();
	}
}