#pragma once
#include <unordered_set>

#include "Entity.h"
#include "AllEnemyWaves.h"
#include "Renderer.h"

#include "AllEntities.h"


class EntityManager
{
private:
	struct RendererAndKeys
	{
		EntityRenderer renderer;
		std::unordered_set<size_t> keys; //TODO: Change into std::vector to benefit from binary_search

		void addEntity(IMovable::RenderInfo renderInfo, size_t key)
		{
			renderer.addEntity(renderInfo, key);
			keys.insert(key);
		}

		void removeEntity(size_t key)
		{
			renderer.removeEntity(key);
			keys.erase(key);
		}
	};

	static int entityCount;

	static unsigned int numEnemies;
	std::unique_ptr<EnemySpawnWave> spawnWave;
	void onWaveStarted();
	void onWaveEnded();

	std::vector<RendererAndKeys> renderLayers = { RendererAndKeys(), RendererAndKeys()};
	std::unordered_map<size_t, std::unique_ptr<Entity>> activeEntities; // Random Access to Entites
	std::unordered_set<size_t> pendingKill;

	// SINGLETON
	EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	RendererAndKeys* getRenderLayerByEnemyKey(const size_t& key);

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

	unsigned int getNumEnemies() const 
		{ return numEnemies; }

	bool entityExists(const size_t id) const;

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
		const size_t entityKey = entityCount++;
		//std::cout << "Using new enemy with ID [" << enemyKey << "]" << std::endl;
		newEntity->setID(entityKey);
		newEntity->spawn(spawnInfo);
		newEntity->enableCollision(); // Add collision

		// Extract render information and pass it to the renderer
		IMovable::RenderInfo renderInfo = newEntity->getRenderInfo();

		const EntityType type = newEntity->getType();

		if (type == EntityType::Enemy)
			numEnemies++;

		// Add to Render Layers
		switch (type)
		{
		case EntityType::Enemy: case EntityType::Pickup:
			renderLayers[0].addEntity(renderInfo, entityKey);
			break;
		case EntityType::Projectile: case EntityType::Particle:
			renderLayers[1].addEntity(renderInfo, entityKey);
			break;
		default:
			break;
		}

		// Actually spawn the enemy properly and update it's attributes accordingly
		activeEntities.emplace(entityKey, std::move(newEntity));
	}

	sf::Drawable* getDrawableLayer(const unsigned int& layer);

	template <typename S>
	void setSpawnWave()
	{
		static_assert(std::is_base_of_v<EnemySpawnWave, S>, "S must be derived from EnemySpawnWave");

		if (spawnWave != nullptr)
			spawnWave.release();
		
		spawnWave = std::make_unique<S>();
	}

	const EnemySpawnWave* getSpawnWave()
	{
		return spawnWave.get();
	}
};