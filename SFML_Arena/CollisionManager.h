#pragma once
#include <unordered_map>
#include <unordered_set>

#include "BaseTypes.h"
#include "Collision.h"

class CollisionManager
{
private:
	static size_t collisionCounter;

	std::unordered_map<EntityType, std::unordered_map<size_t, Collidable*>> collisionLayers;
	Collidable* playerCollision = nullptr;
	std::unordered_map<size_t, Collidable*> projectileCollisions;
	std::unordered_map<size_t, Collidable*> enemyCollisions;
	std::unordered_map<size_t, Collidable*> pickupCollisions;

	// SINGLETON
	CollisionManager();
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	void tick_collision(const float&);

public:
	static CollisionManager& getInstance()
	{
		static CollisionManager instance;
		return instance;
	}

	void registerEntity(Collidable*, const EntityType&);
	void unregisterEntity(const size_t&, const EntityType&);

	void tick(const float& deltaTime);
};