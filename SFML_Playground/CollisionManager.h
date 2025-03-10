#pragma once
#include <unordered_map>
#include <unordered_set>

#include "BaseTypes.h"
#include "Collision.h"

class CollisionManager
{
private:
	static size_t collisionCounter;

	Collidable* playerCollision = nullptr;
	std::unordered_map<size_t, Collidable*> projectileCollisions;
	std::unordered_map<size_t, Collidable*> enemyCollisions;

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

	void unregisterPlayer();
	void unregisterCollidable(const size_t&);
	void registerPlayer(Collidable*);
	void registerEnemy(Collidable*);
	void registerProjectile(Collidable*);

	void tick(const float& deltaTime);
};