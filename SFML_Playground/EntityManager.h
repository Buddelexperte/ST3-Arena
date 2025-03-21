#pragma once

#include "Entity.h"
#include "Renderer.h"

#include "EnemyPool.h"
#include "ProjectilePool.h"

class EntityManager : sf::Drawable
{
private:
	static size_t entityID;

	struct EntityStruct
	{
		std::unique_ptr<Entity> ptr = nullptr;
		EntityType type = EntityType::NoEntity;

		EntityStruct(std::unique_ptr<Entity> ptr, EntityType type)
			: ptr(std::move(ptr)), type(type)
		{ }
	};

	// Pools
	EnemyPool enemyPool; // Manages memory
	ProjectilePool projPool; // Manages memory
	// Renderer
	EntityRenderer renderer; // Manages draw calls

	std::unordered_map<size_t, EntityStruct> activeEntities; // Random Access to Enemies
	std::unordered_set<size_t> pendingKill;

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

	void callDelete(const size_t&);
	void deleteAll();
	void callUpdate(const size_t&, const InfoType&);

	void spawn_Player(const IMovable::RenderInfo& renderInfo);
	void spawn_Enemy(const IMovable::RenderInfo& renderInfo);
	void spawn_Proj(const IMovable::RenderInfo& renderInfo, const float& damage);


	// Call renderer to draw
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};