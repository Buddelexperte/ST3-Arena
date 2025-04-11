#pragma once

#include "EntityManager.h" // Own header file
#include "GameInstance.h"
#include "RandomNumbers.h"
#include "GenericPool.h"
#include "CollisionManager.h"

unsigned int EntityManager::numEnemies = 0;
int EntityManager::entityCount = 0;

EntityManager::EntityManager()
    : spawnWave(std::make_unique<SW_Stage1>())
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

    // Tick the render layers
    tick_renderer(deltaTime);

    CollisionManager::getInstance().tick(deltaTime);

    return;
}

void EntityManager::deleteEntity(const size_t& key)
{
    auto it = activeEntities.find(key);
    if (it != activeEntities.end())
    {
        std::unique_ptr<Entity> entity = std::move(it->second);  // Take ownership
        activeEntities.erase(it);  // Remove from map first to prevent dangling pointers

        if (entity)
        {
            switch (entity->getType())
            {
            case EntityType::Enemy:
                numEnemies--;
                break;
            default:
                break;
            }

            entity->disableCollision();
            entity.release()->releaseToPool();  // Calls derived release function safely
        }
    }

    getRenderLayerByEnemyKey(key)->removeEntity(key);

}

void EntityManager::callDelete(const size_t& key)
{
    pendingKill.insert(key);
}

void EntityManager::callUpdate(const size_t& key, const InfoType& updateFlags)
{
    EntityRenderer* usedRenderLayer = &getRenderLayerByEnemyKey(key)->renderer;

    if (!usedRenderLayer)
        return;

    Entity* entity = activeEntities[key].get();  // Store raw pointer once

    if (updateFlags & InfoType::POSITION) {
        usedRenderLayer->setPosition(key, entity->getPosition());
    }
    if (updateFlags & InfoType::SIZE) {
        usedRenderLayer->setSize(key, entity->getSize());
    }
    if (updateFlags & InfoType::ROTATION) {
        usedRenderLayer->setRotation(key, entity->getRotation());
    }
    if (updateFlags & InfoType::COLOR) {
        usedRenderLayer->setColor(key, entity->getColor());
    }
}

void EntityManager::deleteAll()
{
    for (auto& elem : activeEntities)
    {
        callDelete(elem.first);
    }
}

sf::Drawable* EntityManager::getDrawableLayer(const unsigned int& layer)
{
    if (layer < renderLayers.size())
    {
        return &renderLayers[layer].renderer;
    }

    return nullptr;
}

EntityManager::RendererAndKeys* EntityManager::getRenderLayerByEnemyKey(const size_t& key)
{
    if (renderLayers.empty()) {
        std::cerr << "RenderLayers is empty!" << std::endl;
        return nullptr;
    }

    for (RendererAndKeys& elem : renderLayers)
    {
        if (elem.keys.empty())
            continue;

        if (elem.keys.contains(key))
            return &elem;
    }

    return nullptr;
}

void EntityManager::tick_kill(const float&)
{
    for (const size_t key : pendingKill)
    {
        deleteEntity(key);
    }

    pendingKill.clear();
}

void EntityManager::tick_spawning(const float& deltaTime)
{
    // If spawnWave is valid continue
    if (spawnWave == nullptr)
        return;

    // Check if spawnWave can even spawn right now, if yes, tick it's timer
    if (!spawnWave->canSpawn())
        return;

    // Decrease spawnTimer (Countdown)
    spawnWave->tick_timer(deltaTime);
    spawnWave->trySpawn();
}

void EntityManager::tick_entities(const float& deltaTime)
{
    for (const auto& pair : activeEntities)
    {
        if (!pair.second)
        {
            continue;
        }
        
        pair.second->tick(deltaTime);

        const size_t key = pair.first;
        EntityRenderer* usedRenderLayer = &getRenderLayerByEnemyKey(key)->renderer;

        if (usedRenderLayer == nullptr)
        {
            std::cerr << "Ticking Renderer invalid" << std::endl;
            continue;
        }

        usedRenderLayer->setVelocity(pair.first, pair.second->getVelocity());
        usedRenderLayer->setRotation(pair.first, pair.second->getRotation());
    }
}

void EntityManager::tick_renderer(const float& deltaTime)
{
    for (RendererAndKeys& elem : renderLayers)
    {
        elem.renderer.tick(deltaTime);
    }
}
