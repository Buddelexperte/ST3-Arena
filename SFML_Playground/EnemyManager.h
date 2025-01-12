#pragma once

#include "EnemyPool.h"
#include "EnemyRenderer.h"

class EnemyManager : public sf::Drawable
{
private:
	EnemyPool enemyPool; // Manages memory
	EnemyRenderer enemyRenderer; // Manages draw calls
	std::vector<std::unique_ptr<Enemy>> activeEnemies;

	// Private constructor to prevent direct instantiation
	EnemyManager();
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete; // Block the '=' operator to stop copies being made of this class

public:
	static EnemyManager& getInstance()
	{
		static EnemyManager instance;
		return instance;
	}

	int getActiveEnemies() const;

	void spawnEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
	void deleteEnemy(const size_t& index);

	void tick(const float& deltaTime);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};