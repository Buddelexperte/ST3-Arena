#pragma once
#include <unordered_set>

#include "Enemy.h"
#include "Renderer.h"

class EnemyManager : public sf::Drawable
{
private:
	static unsigned int numEnemies;
	static int enemyID;

	EntityRenderer renderer; // Manages draw calls
	std::unordered_map<size_t, std::unique_ptr<Enemy>> activeEnemies; // Random Access to Enemies
	std::unordered_set<size_t> pendingKill;

	// SINGLETON
	EnemyManager();
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	void tick_kill(const float&);
	sf::Vector2f getNewSpawnPos() const;
	IMovable::RenderInfo makeSpawnRenderInfo();
	void tick_spawning(const float&);
	void tick_enemies(const float&);

	void deleteEnemy(const size_t&);

	static const inline IMovable::RenderInfo standardRenderInfo = { {0.0f, 0.0f}, {100.0f, 100.0f}, 0.0f, {0.0f, 0.0f}, sf::Color::White };

public:
	static EnemyManager& getInstance()
	{
		static EnemyManager instance;
		return instance;
	}

	int getNumActiveEnemies() const;
	void callDelete(const size_t&);
	void callUpdate(const size_t&, const InfoType&);

	void deleteAll();

	void spawnEnemy(const SpawnInformation&);

	void tick(const float& deltaTime);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};