#pragma once

#include "Entity.h"
#include "Renderer.h"

#include "EnemyPool.h"
#include "ProjectilePool.h"

class EntityManager : sf::Drawable
{
private:
	static size_t enemyID;

	EnemyPool enemyPool; // Manages memory
	ProjectilePool projPool; // Manages memory
	EntityRenderer renderer; // Manages draw calls

	std::unordered_map<size_t, std::unique_ptr<Entity>> activeEntities; // Random Access to Enemies
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

	int getNumEntites() const;
	void callDelete(const size_t&);
	void deleteAll();
	void callUpdate(const size_t&, const InfoType&);

	void spawn_Player(const IMovable::RenderInfo& renderInfo);
	void spawn_Enemy(const IMovable::RenderInfo& renderInfo);
	void spawn_Proj(const IMovable::RenderInfo& renderInfo, const float& damage);

	void tick(const float& deltaTime);

	// Call renderer to draw
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};