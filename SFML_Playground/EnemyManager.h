#pragma once

#include "EnemyPool.h"
#include "EnemyRenderer.h"

class EnemyManager : public sf::Drawable
{
private:
	GI_Arena* gameInstance = nullptr;

	const float spawnInterval = 0.00001f;
	const int maxEnemies = 1000; // Temporary safe guard, replace with actual spawning logic later

	EnemyPool enemyPool; // Manages memory
	EnemyRenderer enemyRenderer; // Manages draw calls
	std::vector<std::unique_ptr<Enemy>> activeEnemies; // Random Access to Enemies

	// SINGLETON
	EnemyManager();
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	void tick_spawning(const float& deltaTime);
	void tick_enemies(const float& deltaTime);
public:
	static EnemyManager& getInstance()
	{
		static EnemyManager instance;
		return instance;
	}

	int getNumActiveEnemies() const;
	void callUpdate(const size_t&);

	void spawnEnemy();
	void spawnEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
	void deleteEnemy(const size_t& index);

	void tick(const float& deltaTime);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};