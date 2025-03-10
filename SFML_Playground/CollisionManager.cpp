#pragma once
#include <iostream>

#include "CollisionManager.h"

size_t CollisionManager::collisionCounter = 0;

CollisionManager::CollisionManager()
{

}

void CollisionManager::tick_collision(const float& deltaTime)
{
	for (auto& enemy_pair : enemyCollisions)
	{
		if (!enemy_pair.second)
			continue;

		// Enemy collision with player
		if (playerCollision)
		{
			// Player collides with Enemy
			if (enemy_pair.second->isColliding(playerCollision))
			{
				playerCollision->onCollision(enemy_pair.second);
				enemy_pair.second->onCollision(playerCollision);
			}
		}
		

		// Enemy collision with projectiles
		for (auto& proj_pair : projectileCollisions)
		{
			if (!proj_pair.second)
				continue;

			// Projectile collides with Enemy
			if (enemy_pair.second->isColliding(proj_pair.second))
			{
				enemy_pair.second->onCollision(proj_pair.second);
				proj_pair.second->onCollision(enemy_pair.second);
			}
		}
	}
}

void CollisionManager::unregisterPlayer()
{
	playerCollision = nullptr;
}

void CollisionManager::unregisterCollidable(const size_t& key)
{

	if (projectileCollisions.find(key) != projectileCollisions.end())
		projectileCollisions.erase(key);

	if (enemyCollisions.find(key) != enemyCollisions.end())
		enemyCollisions.erase(key);

	std::cout << "Collision [" << key << "] unregistered" << std::endl;
}

void CollisionManager::registerPlayer(Collidable* collidable)
{
	std::cout << "Registered player Collision [" << collisionCounter << "]" << std::endl;
	playerCollision = collidable;
	collidable->setCollisionID(collisionCounter);
	collisionCounter++;
}

void CollisionManager::registerEnemy(Collidable* collidable)
{
	std::cout << "Registered enemy Collision [" << collisionCounter << "]" << std::endl;
	enemyCollisions[collisionCounter] = collidable;
	collidable->setCollisionID(collisionCounter);
	collisionCounter++;
}

void CollisionManager::registerProjectile(Collidable* collidable)
{
	std::cout << "Registered projectile Collision [" << collisionCounter << "]" << std::endl;
	projectileCollisions[collisionCounter] = collidable;
	collidable->setCollisionID(collisionCounter);
	collisionCounter++;
}

void CollisionManager::tick(const float& deltaTime)
{
	tick_collision(deltaTime);
}