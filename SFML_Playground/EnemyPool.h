#pragma once
#include <memory>
#include <queue>

#include "Enemy.h"

class EnemyPool
{
private:
    std::queue<std::unique_ptr<Enemy>> pool;
public:
    std::unique_ptr<Enemy> get()
    {
        if (pool.empty())
        {
            std::cout << "No enemy left in pool... creating new Enemy" << std::endl;
            return std::make_unique<Enemy>();
        }

        std::unique_ptr<Enemy> enemy = std::move(pool.front());
        std::cout << "Reusing enemy from EnemyPool. Is valid? " << (enemy != nullptr) << std::endl;
        pool.pop();
        return enemy;
    }

    void release(std::unique_ptr<Enemy> enemyPtr)
    {
        std::cout << "Enemy put back in pool. Pool size: " << pool.size() << std::endl;
        pool.push(std::move(enemyPtr));
    }
};