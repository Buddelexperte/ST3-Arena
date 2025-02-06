#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "Enemy.h"

class EnemyRenderer : public sf::Drawable
{
private:
    // Store all enemy quads (4 vertices per enemy)
    sf::VertexArray enemies; // This array will always contain enemyKeys.size() * 4 vertices.

    // Mapping from enemy key to its index in our vectors.
    std::unordered_map<size_t, size_t> keyIndexMap;

    // Vectors holding enemy data in a contiguous manner.
    std::vector<size_t> enemyKeys;                // Index -> key (reverse mapping)
    std::vector<Enemy::RenderInfo> infos;           // Index -> render info
    std::vector<InfoType> updateFlags;              // Index -> update flags

public:
    EnemyRenderer() : enemies(sf::Quads) {}

    // Adds a new enemy and appends its quad to the vertex array.
    void addEnemy(const Enemy::RenderInfo& info, const size_t& key)
    {
        // New enemy index is the current count.
        size_t index = enemyKeys.size();
        enemyKeys.push_back(key);
        infos.push_back(info);
        updateFlags.push_back(InfoType::EMPTY_INFO);

        // Store the mapping from key to index.
        keyIndexMap[key] = index;

        // Expand the vertex array by 4 vertices.
        size_t prevVertexCount = enemies.getVertexCount();
        enemies.resize(prevVertexCount + 4);

        // Compute quad vertices.
        sf::Vertex* quad = &enemies[prevVertexCount];
        const sf::Vector2f pos = info.pos;
        const sf::Vector2f halfSize = info.size / 2.f;

        quad[0].position = pos + sf::Vector2f(-halfSize.x, -halfSize.y);
        quad[1].position = pos + sf::Vector2f(halfSize.x, -halfSize.y);
        quad[2].position = pos + sf::Vector2f(halfSize.x, halfSize.y);
        quad[3].position = pos + sf::Vector2f(-halfSize.x, halfSize.y);

        // Set the same color for all vertices.
        for (int i = 0; i < 4; i++)
        {
            quad[i].color = info.color;
        }
    }

    void setPosition(const size_t key, const sf::Vector2f& position)
    {
        auto it = keyIndexMap.find(key);
        if (it == keyIndexMap.end())
            return; // Key not found

        size_t index = it->second;
        infos[index].pos = position;
        updateFlags[index] |= InfoType::POSITION;
    }

    void setSize(const size_t key, const sf::Vector2f& size)
    {
        auto it = keyIndexMap.find(key);
        if (it == keyIndexMap.end())
            return;

        size_t index = it->second;
        infos[index].size = size;
        updateFlags[index] |= InfoType::SIZE;
    }

    void setVelocity(const size_t key, const sf::Vector2f& velocity)
    {
        auto it = keyIndexMap.find(key);
        if (it == keyIndexMap.end())
            return;

        size_t index = it->second;
        infos[index].velocity = velocity;
        // Velocity doesn't need an update flag since tick() directly applies it.
    }

    void setColor(const size_t key, const sf::Color& color)
    {
        auto it = keyIndexMap.find(key);
        if (it == keyIndexMap.end())
            return;

        size_t index = it->second;
        infos[index].color = color;
        updateFlags[index] |= InfoType::COLOR;
    }

    // Updates the vertex quad at the specified index if needed.
    void updateVertexQuad(size_t index)
    {
        InfoType flags = updateFlags[index];
        size_t vertexStart = index * 4;

        // Update positions if POSITION or SIZE flag is set.
        if ((flags & InfoType::POSITION) != InfoType::EMPTY_INFO ||
            (flags & InfoType::SIZE) != InfoType::EMPTY_INFO)
        {
            const sf::Vector2f pos = infos[index].pos;
            const sf::Vector2f halfSize = infos[index].size / 2.f;
            enemies[vertexStart + 0].position = pos + sf::Vector2f(-halfSize.x, -halfSize.y);
            enemies[vertexStart + 1].position = pos + sf::Vector2f(halfSize.x, -halfSize.y);
            enemies[vertexStart + 2].position = pos + sf::Vector2f(halfSize.x, halfSize.y);
            enemies[vertexStart + 3].position = pos + sf::Vector2f(-halfSize.x, halfSize.y);
        }

        // Update color if COLOR flag is set.
        if ((flags & InfoType::COLOR) != InfoType::EMPTY_INFO)
        {
            const sf::Color color = infos[index].color;
            for (int i = 0; i < 4; i++)
            {
                enemies[vertexStart + i].color = color;
            }
        }
        // Reset update flag.
        updateFlags[index] = InfoType::EMPTY_INFO;
    }

    // Removes an enemy given its key by swapping with the last enemy.
    void removeEnemy(const size_t& key)
    {
        auto it = keyIndexMap.find(key);
        if (it == keyIndexMap.end())
            return; // Enemy not found

        size_t indexToRemove = it->second;
        size_t lastIndex = enemyKeys.size() - 1;

        // Erase the mapping for the enemy to be removed.
        keyIndexMap.erase(it);

        if (indexToRemove != lastIndex)
        {
            // Get the key of the enemy that is currently last.
            size_t swappedKey = enemyKeys[lastIndex];

            // Swap the enemy key.
            enemyKeys[indexToRemove] = enemyKeys[lastIndex];
            enemyKeys.pop_back();

            // Swap the render info.
            infos[indexToRemove] = infos[lastIndex];
            infos.pop_back();

            // Swap the update flags.
            updateFlags[indexToRemove] = updateFlags[lastIndex];
            updateFlags.pop_back();

            // Update the key-index mapping for the swapped enemy.
            keyIndexMap[swappedKey] = indexToRemove;

            // Swap the vertex quads in the vertex array.
            size_t vertexStartRemove = indexToRemove * 4;
            size_t vertexStartLast = lastIndex * 4;
            for (size_t i = 0; i < 4; i++)
            {
                std::swap(enemies[vertexStartRemove + i], enemies[vertexStartLast + i]);
            }
            // Remove the last quad from the vertex array.
            enemies.resize(enemies.getVertexCount() - 4);
        }
        else
        {
            // Removing the last enemy, just pop back.
            enemyKeys.pop_back();
            infos.pop_back();
            updateFlags.pop_back();
            enemies.resize(enemies.getVertexCount() - 4);
        }
    }

    // Tick: update enemy positions based on velocity and apply pending changes.
    void tick(const float& deltaTime)
    {
        // Update positions using velocity.
        for (size_t i = 0; i < enemyKeys.size(); i++)
        {
            sf::Vector2f offset = infos[i].velocity * deltaTime;
            if (offset != sf::Vector2f(0.f, 0.f))
            {
                infos[i].pos += offset;
                size_t vertexStart = i * 4;
                for (size_t j = 0; j < 4; j++)
                {
                    enemies[vertexStart + j].position += offset;
                }
            }
        }

        // Process any queued update flags.
        for (size_t i = 0; i < enemyKeys.size(); i++)
        {
            if (updateFlags[i] != InfoType::EMPTY_INFO)
            {
                updateVertexQuad(i);
            }
        }
    }

    // Draws the complete vertex array at once.
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
    {
        target.draw(enemies, states);
    }
};