#pragma once

#include "EntityManager.h" // Own header file
#include "GenericPool.h"

void EntityManager::tick(const float& deltaTime)
{
    // Kill all pendingKill enemies
    tick_kill(deltaTime);

    // Spawning behavior of Enemies
    tick_spawning(deltaTime);

    // Ticking of each enemy
    tick_entities(deltaTime);

    // Ticking of each renderer layer
    tick_renderer(deltaTime);

    return;
}

void EntityManager::callDelete(const size_t&)
{
}

void EntityManager::callUpdate(const size_t&, const InfoType&)
{
}

void EntityManager::deleteAll()
{
}

void EntityManager::tick_kill(const float&)
{
}

void EntityManager::tick_spawning(const float&)
{
}

void EntityManager::tick_entities(const float&)
{
}

void EntityManager::tick_renderer(const float&)
{
}

void EntityManager::deleteEntity(const size_t&)
{
}
