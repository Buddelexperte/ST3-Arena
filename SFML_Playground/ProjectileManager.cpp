#pragma once

#include "ProjectileManager.h" // Header File
#include "GenericPool.h"

int ProjectileManager::projectileID = 0;

ProjectileManager::ProjectileManager()
{

}

int ProjectileManager::getNumActiveProjectiles() const
{
    return static_cast<int>(activeProjectiles.size());
}

void ProjectileManager::createProjectile(const SpawnInformation& spawnInfo)
{
    // Retrieve an enemy instance from the pool
    std::unique_ptr<Projectile> newProjectile = GenericPool<Projectile>::instance().get();

    // Set the enemy's index and add it to the activeProjectiles vector
    size_t projectileKey = projectileID;
    projectileID++;


    newProjectile->setID(projectileKey);

    newProjectile->spawn(spawnInfo);

    // Extract render information and pass it to the renderer
    IMovable::RenderInfo renderInfo = newProjectile->getRenderInfo();
    renderer.addEntity(renderInfo, projectileKey);

    // Actually spawn the enemy properly and update it's attributes accordingly
    activeProjectiles.emplace(projectileKey, std::move(newProjectile));
}

void ProjectileManager::deleteProjectile(const size_t& key)
{
    CollisionManager::getInstance().unregisterCollidable(activeProjectiles[key]->getCollision()->getCollisionID());
    GenericPool<Projectile>::instance().release(std::move(activeProjectiles[key]));

    auto it = activeProjectiles.find(key);
    if (it != activeProjectiles.end())
    {
        std::unique_ptr<Entity> entity = std::move(it->second);  // Take ownership
        activeProjectiles.erase(it);  // Remove from map first to prevent dangling pointers

        if (entity)
        {
            entity->releaseToPool();  // Calls derived release function safely
        }
    }

    renderer.removeEntity(key);
}

void ProjectileManager::callDelete(const size_t& key)
{
    pendingKill.insert(key);
}

void ProjectileManager::callUpdate(const size_t& key, const InfoType& updateFlags = InfoType::EMPTY_INFO)
{
    if (updateFlags & InfoType::POSITION) { // Check if POSITION flag is set
        renderer.setPosition(key, activeProjectiles[key]->getPosition());
    }
    if (updateFlags & InfoType::SIZE) { // Check if SIZE flag is set
        renderer.setSize(key, activeProjectiles[key]->getSize());
    }
    if (updateFlags & InfoType::COLOR) { // Check if COLOR flag is set
        renderer.setColor(key, activeProjectiles[key]->getColor());
    }
}

void ProjectileManager::deleteAll()
{
    for (const auto& pair : activeProjectiles)
    {
        callDelete(pair.first);
    }
}

void ProjectileManager::tick_kill(const float& deltaTime)
{
    for (const size_t& key : pendingKill)
    {
        deleteProjectile(key);
    }
    pendingKill.clear();
}

void ProjectileManager::tick_projectiles(const float& deltaTime)
{
    for (const auto& pair : activeProjectiles)
    {
        if (!pair.second)
            continue;

        pair.second->tick(deltaTime);
        renderer.setVelocity(pair.first, pair.second->getVelocity());
        renderer.setRotation(pair.first, pair.second->getRotation());
    }

}

void ProjectileManager::tick(const float& deltaTime)
{
    // Kill all pendingKill enemies
    tick_kill(deltaTime);

    // Ticking of each enemy
    tick_projectiles(deltaTime);

    // Update the enemy renderer at last
    renderer.tick(deltaTime);
}

void ProjectileManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    renderer.draw(target, states);
}