#pragma once
#include <memory>
#include <queue>

#include "Projectile.h"

class ProjectilePool
{
private:
    std::queue<std::unique_ptr<Projectile>> pool;
public:
    std::unique_ptr<Projectile> get()
    {
        if (pool.empty())
        {
            return std::make_unique<Projectile>();
        }

        std::unique_ptr<Projectile> projectile = std::move(pool.front());
        pool.pop();
        return projectile;
    }

    void release(std::unique_ptr<Projectile> enemyPtr)
    {
        pool.push(std::move(enemyPtr));
    }
};