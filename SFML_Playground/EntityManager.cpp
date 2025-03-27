#pragma once

#include "EntityManager.h" // Own header file
#include "GameInstance.h"
#include "RandomNumbers.h"
#include "GenericPool.h"
#include "CollisionManager.h"

unsigned int EntityManager::numEnemies = 0;
int EntityManager::entityID = 0;

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

    // Tick the render layers
    tick_renderer(deltaTime);

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

// TODO: Make EnemySpawner class instead of in EntityManager!

sf::Vector2f EntityManager::getNewSpawnPos() const
{
    constexpr float MIN_DISTANCE = 800.0; // 800
    constexpr float MAX_DISTANCE = 1200.0f; // 1200

    // Getting values for generating a position
    sf::Vector2f playerPos = gameInstance().getPlayer()->getPosition();

    // Generating the random position
    float distance = RNG::floatInRange(MIN_DISTANCE, MAX_DISTANCE);
    sf::Vector2f generatedPos = RNG::posInDistanceFrom(playerPos, distance);

    return generatedPos;
}

IMovable::RenderInfo EntityManager::makeSpawnRenderInfo()
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

    if (numEnemies == 0)
        renderInfo.color = sf::Color::Red;

    return renderInfo;
}

void EntityManager::tick_spawning(const float& deltaTime)
{
    constexpr float SPAWN_INTERVAL = SMALLEST_PRECISION;
    constexpr unsigned int MAX_ENEMIES = 20; // Temporary safe guard, replace with actual spawning logic later

    // TODO: Temporary values, replace with spawning logic in EnemySpawner class
    constexpr float ENEMY_HEALTH = 1.0f;
    constexpr float ENEMY_DAMAGE = 0.1f;

    static ValueBar timer(SPAWN_INTERVAL); // Static variable to track time between enemy spawns.

    // If the number of active enemies would exceed the maximum allowed cancel any spawning attempt
    if (numEnemies >= MAX_ENEMIES)
        return;

    // Decrease spawnTimer (Countdown)
    timer.addValue(-deltaTime);
    // If the timer did not reach zero, cancel spawn attempt
    if (timer.isEmpty())
    {
        SpawnInformation spawnInfo = { makeSpawnRenderInfo(), ENEMY_HEALTH, ENEMY_DAMAGE };
        spawnEntity<Enemy>(spawnInfo);
        timer.reset(); // Reset the timer to the configured spawn interval
    }
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
