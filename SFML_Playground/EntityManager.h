#pragma once
#include <unordered_set>

#include "Enemy.h"
#include "Renderer.h"

class EntityManager
{
private:
	static unsigned int numEnemies;
	static int entityID;

	std::vector<EntityRenderer> renderLayers;
	std::unordered_map<size_t, std::unique_ptr<Enemy>> activeEnemies; // Random Access to Enemies
	std::unordered_set<size_t> pendingKill;

	// SINGLETON
	EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	void tick_kill(const float&);
	void tick_spawning(const float&);
	void tick_enemies(const float&);

	void deleteEnemy(const size_t&);

	static const inline IMovable::RenderInfo standardRenderInfo = { {0.0f, 0.0f}, {100.0f, 100.0f}, 0.0f, {0.0f, 0.0f}, sf::Color::White };

public:
	static EntityManager& getInstance()
	{
		static EntityManager instance;
		return instance;
	}

	void callDelete(const size_t&);
	void callUpdate(const size_t&, const InfoType&);

	void deleteAll();

	void spawnEnemy();

	void tick(const float& deltaTime);
};