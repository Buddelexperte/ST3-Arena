#pragma once
#include <memory>
#include <queue>

#include "Player.h"

class PlayerPool
{
private:
    std::queue<std::unique_ptr<Player>> pool;
public:
    std::unique_ptr<Player> get()
    {
        if (pool.empty())
        {
            return std::make_unique<Player>();
        }

        std::unique_ptr<Player> player = std::move(pool.front());
        pool.pop();
        return player;
    }

    void release(std::unique_ptr<Player> playerPtr)
    {
        pool.push(std::move(playerPtr));
    }
};