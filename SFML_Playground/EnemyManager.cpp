#pragma once

#include "EnemyManager.h" // Header File

unsigned int EnemyManager::numEnemies = 0;
int EnemyManager::enemyID = 0;

EnemyManager::EnemyManager()
{

}

int EnemyManager::getNumActiveEnemies() const
{
	return static_cast<int>(activeEnemies.size());
}

void EnemyManager::spawnEnemy()
{
    // Retrieve an enemy instance from the pool
    std::unique_ptr<Enemy> newEnemy = enemyPool.get();

    // Set the enemy's index and add it to the activeEnemies vector
    size_t enemyKey = enemyID;
    enemyID++;
    numEnemies++;
    //std::cout << "Using new enemy with ID [" << enemyKey << "]" << std::endl;
    newEnemy->setID(enemyKey);

    newEnemy->spawn();

    // Extract render information and pass it to the renderer
    IMovable::RenderInfo renderInfo = newEnemy->getRenderInfo();
    renderer.addEntity(renderInfo, enemyKey);

    // Actually spawn the enemy properly and update it's attributes accordingly
    activeEnemies[enemyKey] = (std::move(newEnemy));
}

void EnemyManager::deleteEnemy(const size_t& key)
{
    CollisionManager::getInstance().unregisterCollidable(activeEnemies[key]->getCollision()->getCollisionID());
    enemyPool.release(std::move(activeEnemies[key]));
    
    activeEnemies.erase(key);

    renderer.removeEntity(key);

    numEnemies--;
}

void EnemyManager::callDelete(const size_t& key)
{
    pendingKill.insert(key);
}

void EnemyManager::deleteAll()
{
	for (const auto& pair : activeEnemies)
	{
		callDelete(pair.first);
	}
}

void EnemyManager::callUpdate(const size_t& key, const InfoType& updateFlags = InfoType::EMPTY_INFO)
{
    if (updateFlags & InfoType::POSITION) { // Check if POSITION flag is set
        renderer.setPosition(key, activeEnemies[key]->getPosition());
    }
    if (updateFlags & InfoType::SIZE) { // Check if SIZE flag is set
        renderer.setSize(key, activeEnemies[key]->getSize());
    }
    if (updateFlags & InfoType::COLOR) { // Check if COLOR flag is set
        renderer.setColor(key, activeEnemies[key]->getColor());
    }
}

void EnemyManager::tick_kill(const float& deltaTime)
{
    for (const size_t& key : pendingKill)
    {
        deleteEnemy(key);
    }
    pendingKill.clear();
}

void EnemyManager::tick_spawning(const float& deltaTime)
{
    constexpr float SPAWN_INTERVAL = SMALLEST_PRECISION;
    constexpr unsigned int MAX_ENEMIES = 9; // Temporary safe guard, replace with actual spawning logic later

    static ValueBar timer(SPAWN_INTERVAL); // Static variable to track time between enemy spawns.

    // If the number of active enemies would exceed the maximum allowed cancel any spawning attempt
    if (numEnemies >= MAX_ENEMIES)
        return;

    // Decrease spawnTimer (Countdown)
    timer.addValue(-deltaTime);
    // If the timer did not reach zero, cancel spawn attempt
    if (timer.isEmpty())
    {
        spawnEnemy();
        timer.reset(); // Reset the timer to the configured spawn interval
    }
}

void EnemyManager::tick_enemies(const float& deltaTime)
{
    for (const auto& pair : activeEnemies)
    {
        if (!pair.second)
            continue;

        pair.second->tick(deltaTime);
        renderer.setVelocity(pair.first, pair.second->getVelocity());
        renderer.setRotation(pair.first, pair.second->getRotation());
    }
}

void EnemyManager::tick(const float& deltaTime)
{
    // Kill all pendingKill enemies
    tick_kill(deltaTime);

    // Spawning behavior of Enemies
    tick_spawning(deltaTime);

    // Ticking of each enemy
    tick_enemies(deltaTime);

    // Update the enemy renderer at last
    renderer.tick(deltaTime);
}

void EnemyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	renderer.draw(target, states);
}