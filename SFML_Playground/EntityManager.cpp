#pragma once
#include <cassert>

#include "EntityManager.h"

size_t EntityManager::entityID = 0;
unsigned int EntityManager::numEnemies = 0;

EntityManager::EntityManager()
{

}

void EntityManager::tick(const float& deltaTime)
{
    // Kill all pendingKill enemies
    tick_kill(deltaTime);

    // Spawning behavior of Enemies
    tick_spawning(deltaTime);

    // Ticking of each enemy
    tick_entities(deltaTime);

    // Update the enemy renderer at last
    renderer.tick(deltaTime);
}

void EntityManager::tick_kill(const float& deltaTime)
{
    for (const size_t& key : pendingKill)
    {
        deleteEntity(key);
    }
    pendingKill.clear();
}

void EntityManager::tick_spawning(const float& deltaTime)
{
    constexpr float SPAWN_INTERVAL = SMALLEST_PRECISION;
    constexpr unsigned int MAX_ENEMIES = 9;

    static ValueBar timer(SPAWN_INTERVAL); // Static variable to track time between enemy spawns.

    // If the number of active enemies would exceed the maximum allowed cancel any spawning attempt
    if (numEnemies >= MAX_ENEMIES)
        return;

    // Decrease spawnTimer (Countdown)
    timer.addValue(-deltaTime);
    // If the timer did not reach zero, cancel spawn attempt
    if (timer.isEmpty())
    {
        spawn(EntityType::Enemy);
        timer.reset(); // Reset the timer to the configured spawn interval
    }
}

void EntityManager::tick_entities(const float& deltaTime)
{
    for (const auto& pair : activeEntities)
    {
        if (!pair.second.ptr)
            continue;

        pair.second.ptr->tick(deltaTime);
        renderer.setVelocity(pair.first, pair.second.ptr->getVelocity());
        renderer.setRotation(pair.first, pair.second.ptr->getRotation());
    }
}

void EntityManager::deleteEntity(const size_t& key)
{
    // Move the unique_ptr out of the active entities container.
    std::unique_ptr<Entity> entity = std::move(activeEntities[key].ptr);
    EntityType type = activeEntities[key].type;

    // Remove the key from the active entities.
    activeEntities.erase(key);

    switch (type)
    {
    case EntityType::Player:
    {
        // We are sure that the entity is a Player, so use static_cast.
        Player* playerPtr = static_cast<Player*>(entity.release());
        assert(playerPtr != nullptr); // Sanity check in debug builds.
        playerPool.release(std::unique_ptr<Player>(playerPtr));
        break;
    }
    case EntityType::Enemy:
    {
        // We are sure that the entity is an Enemy, so use static_cast.
        Enemy* enemyPtr = static_cast<Enemy*>(entity.release());
        assert(enemyPtr != nullptr);
        enemyPool.release(std::unique_ptr<Enemy>(enemyPtr));
        numEnemies--;
        break;
    }
    case EntityType::Projectile:
    {
        // We are sure that the entity is a Projectile, so use static_cast.
        Projectile* projPtr = static_cast<Projectile*>(entity.release());
        assert(projPtr != nullptr);
        projPool.release(std::unique_ptr<Projectile>(projPtr));
        break;
    }
    default:
        break;
    }
}

size_t EntityManager::getNumEntites() const
{
	return activeEntities.size();
}


void EntityManager::callDelete(const size_t& key)
{
    pendingKill.insert(key);
}


void EntityManager::deleteAll()
{
    for (const auto& pair : activeEntities)
    {
        callDelete(pair.first);
    }
}

void EntityManager::callUpdate(const size_t& key, const InfoType& updateFlags = InfoType::EMPTY_INFO)
{
    if (updateFlags & InfoType::POSITION) { // Check if POSITION flag is set
        renderer.setPosition(key, activeEntities[key].ptr->getPosition());
    }
    if (updateFlags & InfoType::SIZE) { // Check if SIZE flag is set
        renderer.setSize(key, activeEntities[key].ptr->getSize());
    }
    if (updateFlags & InfoType::COLOR) { // Check if COLOR flag is set
        renderer.setColor(key, activeEntities[key].ptr->getColor());
    }
}

void EntityManager::spawn(EntityType type)
{
    std::unique_ptr<Entity> newEntity = nullptr;

    switch (type)
    {
    case EntityType::Player:
        newEntity = playerPool.get();
        break;
    case EntityType::Enemy:
        newEntity = enemyPool.get();
        numEnemies++;
        break;
    case EntityType::Projectile:
        newEntity = projPool.get();
        break;
    default:
        return;
        break;
    }

    // Set the entity's key
    size_t key = entityID;
    entityID++;
    //std::cout << "Using new entity with ID [" << key << "]" << std::endl;
    newEntity->setID(key);
    //std::cout << "Set ID to corresponding key" << std::endl;
    newEntity->spawn();
    //std::cout << "Spawned entity" << std::endl;


    // Extract render information and pass it to the renderer
    IMovable::RenderInfo renderInfo = newEntity->getRenderInfo();
    renderer.addEntity(renderInfo, key);

    activeEntities.emplace(key, EntityStruct(std::move(newEntity), type));
}

void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	renderer.draw(target, states);
}