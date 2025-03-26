#pragma once
#include <unordered_set>

#include "Entity.h"
#include "Renderer.h"

struct RendererAndKeys
{
	EntityRenderer renderer;
	std::unordered_set<size_t> keys;
};

class EntityManager
{
private:
	static unsigned int numEnemies;
	static int entityID;

	std::vector<RendererAndKeys> renderLayers;
	std::unordered_map<size_t, std::unique_ptr<Entity>> activeEnemies; // Random Access to Enemies
	std::unordered_set<size_t> pendingKill;

	// SINGLETON
	EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	// Tick wrappers
	void tick_kill(const float&);
	void tick_spawning(const float&);
	void tick_entities(const float&);
	void tick_renderer(const float&);

	void deleteEntity(const size_t&);

public:
	static EntityManager& getInstance()
	{
		static EntityManager instance;
		return instance;
	}

	void tick(const float& deltaTime);

	void callDelete(const size_t&);
	void callUpdate(const size_t&, const InfoType&);

	void deleteAll();

	template <typename T>
	void spawnEntity(const SpawnInformation& spawnInfo)
	{
		static_assert(std::is_base_of_v<Entity, T>, "T must be derived from Entity");

		// Retrieve an enemy instance from the pool
		std::unique_ptr<Entity> newEntity = GenericPool<T>::instance().get();

		// Set the enemy's index and add it to the activeEnemies vector
		const size_t enemyKey = entityID++;
		//std::cout << "Using new enemy with ID [" << enemyKey << "]" << std::endl;
		newEntity->setID(enemyKey);
		newEntity->spawn(spawnInfo);

		// Extract render information and pass it to the renderer
		IMovable::RenderInfo renderInfo = newEntity->getRenderInfo();

		switch (newEntity->getType())
		{
		case EntityType::Enemy:
			numEnemies++;
			renderLayers[0].renderer.addEntity(renderInfo, enemyKey);
			break;
		case EntityType::Projectile:
			renderLayers[0].renderer.addEntity(renderInfo, enemyKey);
			break;
		default:
			break;
		}

		// Actually spawn the enemy properly and update it's attributes accordingly
		activeEnemies.emplace(enemyKey, std::move(newEntity));
	}
};