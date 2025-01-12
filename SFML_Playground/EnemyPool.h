#pragma once
#include <memory>
#include <queue>

#include <iostream>

#include "Enemy.h"

class EnemyPool
{
private:
    std::queue<std::unique_ptr<Enemy>> pool;

public:
    std::unique_ptr<Enemy> get(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
    {
        if (pool.empty())
            return std::make_unique<Enemy>(pos, size, color);
        std::unique_ptr<Enemy> enemy = std::move(pool.front());
        pool.pop();
        enemy->setPosition(pos);
        enemy->setSize(size);
        enemy->setColor(color);
        return enemy;
    }

    void release(std::unique_ptr<Enemy> enemyPtr)
    {
        pool.push(std::move(enemyPtr));
    }
};