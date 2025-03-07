#pragma once
#include <unordered_set>

#include "GameInstance.h"
#include "EnemyPool.h"
#include "EnemyRenderer.h"
#include "ProjectileManager.h" // For projectile collision

class EnemyManager : public sf::Drawable
{
private:
	GI_Arena& gameInstance = GI_Arena::getInstance();
	ProjectileManager& projectileManager = ProjectileManager::getInstance();

	const float spawnInterval = SMALLEST_PRECISION;
	const int maxEnemies = 9; // Temporary safe guard, replace with actual spawning logic later
	static int enemyID;

	EnemyPool enemyPool; // Manages memory
	EnemyRenderer enemyRenderer; // Manages draw calls
	std::unordered_map<size_t, std::unique_ptr<Enemy>> activeEnemies; // Random Access to Enemies
	std::unordered_set<size_t> pendingKill;

	// SINGLETON
	EnemyManager();
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	void tick_kill(const float&);
	void tick_spawning(const float&);
	void tick_enemies(const float&);

	void deleteEnemy(const size_t&);

	const IMovable::RenderInfo standardRenderInfo = { {0.0f, 0.0f}, {100.0f, 100.0f}, 0.0f, {0.0f, 0.0f}, sf::Color::White };

public:
	static EnemyManager& getInstance()
	{
		static EnemyManager instance;
		return instance;
	}

	void setPlayer(Player*);
	int getNumActiveEnemies() const;
	void callDelete(const size_t&);
	void callUpdate(const size_t&, const InfoType&);

	void spawnEnemy(const IMovable::RenderInfo& renderInfo);

	void tick(const float& deltaTime);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};