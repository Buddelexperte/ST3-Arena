#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <vector>

#include "Enemy.h"

class EnemyManager : public sf::Drawable
{
private:
	EnemyPool enemyPool; // Manages memory
	EnemyRenderer enemyRenderer; // Manages draw calls
	std::vector<std::unique_ptr<Enemy>> activeEnemies;

public:
	EnemyManager();

	void spawnEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
	void deleteEnemy(const size_t& index);

	void tick(const float& deltaTime);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};