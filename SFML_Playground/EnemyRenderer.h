#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>
#include "Enemy.h"

class EnemyRenderer : public sf::Drawable
{
private:
    sf::VertexArray enemies; // Store enemies as quads
    std::vector<Enemy::EnemyRenderInfo> infos; // All enemy velocities
    std::unordered_set<size_t> changedIndices;
    size_t numEnemies = 0; // Track the number of active enemies

public:
    EnemyRenderer()
        : enemies(sf::Quads)
    {}

    void addEnemy(const Enemy::EnemyRenderInfo& info)
    {
        addEnemy(info.pos, info.size, info.velocity, info.color);
    }

    void addEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& velocity, const sf::Color& color)
    {
        size_t index;

        // Check if we have any removed enemies (reusing slot)
        if (numEnemies < enemies.getVertexCount() / 4) {
            // Reuse an index if there are removed enemies
            index = numEnemies;
        }
        else {
            // Add a new enemy if we have no removed enemies to reuse
            index = numEnemies;
            enemies.append(sf::Vertex());
            enemies.append(sf::Vertex());
            enemies.append(sf::Vertex());
            enemies.append(sf::Vertex());
            infos.push_back(Enemy::EnemyRenderInfo(pos, size, velocity, color));
        }

        // Set the position, size, and color for the new enemy
        sf::Vertex v1, v2, v3, v4;

        // Quad vertices
        v1.position = pos + sf::Vector2f(-size.x / 2.f, -size.y / 2.f);
        v2.position = pos + sf::Vector2f(size.x / 2.f, -size.y / 2.f);
        v3.position = pos + sf::Vector2f(size.x / 2.f, size.y / 2.f);
        v4.position = pos + sf::Vector2f(-size.x / 2.f, size.y / 2.f);

        // Color all vertices
        v1.color = v2.color = v3.color = v4.color = color;

        // Set the vertex positions
        enemies[index * 4 + 0] = v1;
        enemies[index * 4 + 1] = v2;
        enemies[index * 4 + 2] = v3;
        enemies[index * 4 + 3] = v4;

        // Set Info for the enemy
        infos[index].pos = pos;
        infos[index].size = size;
        infos[index].velocity = velocity;
        infos[index].color = color;

        // Increment the number of active enemies
        numEnemies++;
    }

    void setPosition(size_t index, const sf::Vector2f& position)
    {
        if (index < infos.size())
        {
            infos[index].pos = position;
            changedIndices.insert(index);
        }
    }
    void setSize(size_t index, const sf::Vector2f& size)
    {
        if (index < infos.size())
        {
            infos[index].size = size;
            changedIndices.insert(index);
        }
    }
    void setVelocity(size_t index, const sf::Vector2f& velocity)
    {
        if (index < infos.size())
        {
            infos[index].velocity = velocity;
        }
    }
    void setColor(size_t index, const sf::Color& color)
    {
        if (index < infos.size())
        {
            infos[index].color = color;
            changedIndices.insert(index);
        }
    }

    void updateVertexQuad(size_t index)
    {
        const sf::Vector2f pos = infos[index].pos;
        const sf::Vector2f size = infos[index].size;
        const sf::Color color = infos[index].color;

        // Quad vertices
        enemies[index * 4 + 0].position = pos + sf::Vector2f(-size.x / 2.f, -size.y / 2.f);
        enemies[index * 4 + 1].position = pos + sf::Vector2f(size.x / 2.f, -size.y / 2.f);
        enemies[index * 4 + 2].position = pos + sf::Vector2f(size.x / 2.f, size.y / 2.f);
        enemies[index * 4 + 3].position = pos + sf::Vector2f(-size.x / 2.f, size.y / 2.f);

        // Color all vertices
        enemies[index * 4 + 0].color = color;
        enemies[index * 4 + 1].color = color;
        enemies[index * 4 + 2].color = color;
        enemies[index * 4 + 3].color = color;
    }

    void removeEnemy(size_t index)
    {
        if (index < numEnemies) {
            // Swap with the last active enemy
            size_t lastIndex = numEnemies - 1;

            // Swap vertices
            for (size_t i = 0; i < 4; i++)
            {
                std::swap(enemies[index * 4 +
                    i], enemies[lastIndex * 4 + i]);
            }

            // Swap velocities
            std::swap(infos[index], infos[lastIndex]);

            // Decrease the number of active enemies
            numEnemies--;
        }
    }

    void tick(const float& deltaTime)
    {
        // Update only the changed indices
        for (size_t i : changedIndices)
        {
            updateVertexQuad(i);
        }

        // Cache velocities to avoid redundant multiplications
        std::vector<sf::Vector2f> offsets(numEnemies);
        for (size_t i = 0; i < numEnemies; i++)
        {
            offsets[i] = infos[i].velocity * deltaTime;
        }

        // Apply the cached offsets to all vertices
        for (size_t i = 0; i < numEnemies; i++)
        {
            const sf::Vector2f& offset = offsets[i];

            // Skip stationary enemies
            if (offset == sf::Vector2f(0.0f, 0.0f))
                continue;

            for (size_t j = 0; j < 4; j++)
            {
                enemies[i * 4 + j].position += offset;
            }
        }

        changedIndices.clear();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
    {
        for (size_t i = 0; i < numEnemies; i++)
        {
            target.draw(&enemies[i * 4], 4, sf::Quads, states);
        }
    }
};
