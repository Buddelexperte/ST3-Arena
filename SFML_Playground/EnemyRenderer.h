#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "Enemy.h"

class EnemyRenderer : public sf::Drawable
{
private:
    sf::VertexArray enemies; // Store enemies as quads
    std::vector<Enemy::EnemyRenderInfo> infos; // All enemy velocities
    std::unordered_map<size_t, InfoType> updateMap;
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
        size_t index = numEnemies;

        // Check if we have any removed enemies (reusing slot)
        if (numEnemies >= enemies.getVertexCount() / 4)
        {
            // Add a new enemy if we have no removed enemies to reuse
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
            updateMap[index] = static_cast<InfoType>(updateMap[index] | InfoType::POSITION);
        }
    }
    void setSize(size_t index, const sf::Vector2f& size)
    {
        if (index < infos.size())
        {
            infos[index].size = size;
            updateMap[index] = static_cast<InfoType>(updateMap[index] | InfoType::SIZE);
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
            updateMap[index] = static_cast<InfoType>(updateMap[index] | InfoType::COLOR);
        }
    }

    void updateVertexQuad(const std::pair<const size_t, InfoType>& pair)
    {
        updateVertexQuad(pair.first, pair.second);
    }

    void updateVertexQuad(size_t index, const InfoType& updateFlags)
    {
        // Quad vertices
        if (updateFlags & InfoType::POSITION || updateFlags & InfoType::SIZE)
        {
            const sf::Vector2f pos = infos[index].pos;
            const sf::Vector2f halfSize = infos[index].size / 2.0f;
            enemies[index * 4 + 0].position = pos + sf::Vector2f(-halfSize.x, -halfSize.y);
            enemies[index * 4 + 1].position = pos + sf::Vector2f(halfSize.x, -halfSize.y);
            enemies[index * 4 + 2].position = pos + sf::Vector2f(halfSize.x, halfSize.y);
            enemies[index * 4 + 3].position = pos + sf::Vector2f(-halfSize.x, halfSize.y);
        }

        // Color all vertices
        if (updateFlags & InfoType::COLOR)
        {
            const sf::Color color = infos[index].color;
            enemies[index * 4 + 0].color = color;
            enemies[index * 4 + 1].color = color;
            enemies[index * 4 + 2].color = color;
            enemies[index * 4 + 3].color = color;
        }
    }

    void removeEnemy(size_t index)
    {
        if (index < numEnemies) {
            // Swap with the last active enemy
            size_t lastIndex = numEnemies - 1;

            // Swap vertices
            for (size_t i = 0; i < 4; i++)
            {
                std::swap(enemies[index * 4 + i], enemies[lastIndex * 4 + i]);
            }

            // Swap velocities
            std::swap(infos[index], infos[lastIndex]);

            // Decrease the number of active enemies
            numEnemies--;
        }
    }

    void tick(const float& deltaTime)
    {
        for (size_t i = 0; i < numEnemies; i++)
        {
            const sf::Vector2f& offset = infos[i].velocity * deltaTime;

            // Skip stationary enemies
            if (offset == sf::Vector2f(0.0f, 0.0f))
                continue;

            infos[i].pos += offset;

            for (size_t j = 0; j < 4; j++)
            {
                enemies[i * 4 + j].position += offset;
            }

        }

        // Update only the changed indices
        for (std::pair<const size_t, InfoType>& pair : updateMap)
        {
            updateVertexQuad(pair);
            updateMap[pair.first] = InfoType::EMPTY_INFO;
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
    {
        for (size_t i = 0; i < numEnemies; i++)
        {
            target.draw(&enemies[i * 4], 4, sf::Quads, states);
        }
    }
};
