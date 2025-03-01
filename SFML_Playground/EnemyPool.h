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
            return std::make_unique<Enemy>();

        std::unique_ptr<Enemy> enemy = std::move(pool.front());
        pool.pop();
        return enemy;
    }

    void release(std::unique_ptr<Enemy> enemyPtr)
    {
        pool.push(std::move(enemyPtr));
    }
};