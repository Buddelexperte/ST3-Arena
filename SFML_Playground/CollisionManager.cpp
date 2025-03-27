#pragma once
#include <iostream>

#include "CollisionManager.h"

size_t CollisionManager::collisionCounter = 0;

CollisionManager::CollisionManager()
{

}

void CollisionManager::tick_collision(const float& deltaTime)
{
	// First calc pickups
	for (auto& pickupPair : pickupCollisions)
	{
		if (!pickupPair.second)
			continue;

		// Pickup collision with player
		if (playerCollision)
		{
			// Player collides with Pickup
			if (pickupPair.second->isColliding(playerCollision))
			{
				playerCollision->onCollision(pickupPair.second);
				pickupPair.second->onCollision(playerCollision);
			}
		}
	}

	// Then calc enemy/proj hits
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

void CollisionManager::registerEntity(Collidable* collidable, const EntityType& type)
{
	switch (type)
	{
	case EntityType::Player:
		playerCollision = collidable;
		break;
	case EntityType::Enemy:
		enemyCollisions[collisionCounter] = collidable;
		break;
	case EntityType::Projectile:
		projectileCollisions[collisionCounter] = collidable;
		break;
	case EntityType::Pickup:
		pickupCollisions[collisionCounter] = collidable;
		break;
	default:
		break;
	}

	collidable->setCollisionID(collisionCounter);
	collisionCounter++;
}

void CollisionManager::unregisterEntity(const size_t& collisionID, const EntityType& type)
{
	switch (type)
	{
	case EntityType::Player:
		playerCollision = nullptr;
		break;
	case EntityType::Enemy:
		if (enemyCollisions.find(collisionID) != enemyCollisions.end())
		{
			enemyCollisions.erase(collisionID);
		}
		break;
	case EntityType::Projectile:
		if (projectileCollisions.find(collisionID) != projectileCollisions.end())
		{
			projectileCollisions.erase(collisionID);
		}
		break;
	case EntityType::Pickup:
		if (pickupCollisions.find(collisionID) != pickupCollisions.end())
		{
			pickupCollisions.erase(collisionID);
		}
		break;
	default:
		break;
	}
}

void CollisionManager::tick(const float& deltaTime)
{
	tick_collision(deltaTime);
}