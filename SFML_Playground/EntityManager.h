#pragma once
#include <unordered_set>

#include "Entity.h"
#include "Renderer.h"

#include "EnemyPool.h"
#include "ProjectilePool.h"
#include "PlayerPool.h"

struct EntityStruct
{
	std::unique_ptr<Entity> ptr = nullptr;
	EntityType type = EntityType::NoEntity;

	EntityStruct()
		: ptr(nullptr), type(EntityType::NoEntity)
	{ }

	EntityStruct(std::unique_ptr<Entity> ptr, EntityType type)
		: ptr(std::move(ptr)), type(type)
	{ }
};

class EntityManager : sf::Drawable
{
private:
	static size_t entityID;


	// Pools
	EnemyPool enemyPool; // Manages memory
	ProjectilePool projPool; // Manages memory
	PlayerPool playerPool; // Manages memory
	// Renderer
	EntityRenderer renderer; // Manages draw calls

	std::unordered_map<size_t, EntityStruct> activeEntities; // Random Access to Enemies
	std::unordered_set<size_t> pendingKill;

	static unsigned int numEnemies;

	// SINGLETON
	EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	void tick_kill(const float&);
	void tick_spawning(const float&);
	void tick_entities(const float&);

	void deleteEntity(const size_t&);

public:
	static EntityManager& getInstance()
	{
		static EntityManager instance;
		return instance;
	}

	size_t getNumEntites() const;

	void tick(const float& deltaTime);

	void spawn(EntityType);
	void callDelete(const size_t&);
	void callUpdate(const size_t&, const InfoType&);

	void deleteAll();

	// Call renderer to draw
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

inline static EntityManager& entityManager()
{
	return EntityManager::getInstance();
}