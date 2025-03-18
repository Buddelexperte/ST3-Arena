#pragma once

#include "ProjectileManager.h" // Header File

int ProjectileManager::projectileID = 0;

ProjectileManager::ProjectileManager()
	: projectileRenderer()
{}

int ProjectileManager::getNumActiveProjectiles() const
{
    return static_cast<int>(activeProjectiles.size());
}

void ProjectileManager::createProjectile(const IMovable::RenderInfo& renderInfo, const float& damage)
{
    // Retrieve an enemy instance from the pool
    std::unique_ptr<Projectile> newProjectile = projectilePool.get();
    // Set the enemy's index and add it to the activeProjectiles vector
    size_t projectileKey = projectileID++;
    newProjectile->setID(projectileKey);

    newProjectile->setRenderInfo(renderInfo);
    newProjectile->setDamage(damage);

    // Extract render information and pass it to the renderer
    projectileRenderer.addProjectile(renderInfo, projectileKey);

    // Actually spawn the enemy properly and update it's attributes accordingly
    activeProjectiles[projectileKey] = (std::move(newProjectile));
    activeProjectiles[projectileKey]->spawn();
}

void ProjectileManager::deleteProjectile(const size_t& key)
{
    CollisionManager::getInstance().unregisterCollidable(activeProjectiles[key]->getCollision()->getCollisionID());
    projectilePool.release(std::move(activeProjectiles[key]));

    activeProjectiles.erase(key);

    projectileRenderer.removeProjectile(key);
}

void ProjectileManager::callDelete(const size_t& key)
{
    pendingKill.insert(key);
}

void ProjectileManager::callUpdate(const size_t& key, const InfoType& updateFlags = InfoType::EMPTY_INFO)
{
    if (updateFlags & InfoType::POSITION) { // Check if POSITION flag is set
        projectileRenderer.setPosition(key, activeProjectiles[key]->getPosition());
    }
    if (updateFlags & InfoType::SIZE) { // Check if SIZE flag is set
        projectileRenderer.setSize(key, activeProjectiles[key]->getSize());
    }
    if (updateFlags & InfoType::COLOR) { // Check if COLOR flag is set
        projectileRenderer.setColor(key, activeProjectiles[key]->getColor());
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
        projectileRenderer.setVelocity(pair.first, pair.second->getVelocity());
        projectileRenderer.setRotation(pair.first, pair.second->getRotation());
    }

}

void ProjectileManager::tick(const float& deltaTime)
{
    // Kill all pendingKill enemies
    tick_kill(deltaTime);

    // Ticking of each enemy
    tick_projectiles(deltaTime);

    // Update the enemy renderer at last
    projectileRenderer.tick(deltaTime);
}

void ProjectileManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    projectileRenderer.draw(target, states);
}