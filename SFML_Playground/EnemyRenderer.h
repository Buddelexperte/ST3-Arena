#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "Enemy.h"

class EnemyRenderer : public sf::Drawable
{
private:
    // Store all enemy quads (4 vertices per enemy)
    sf::VertexArray enemyVertices; // This array will always contain enemyKeys.size() * 4 vertices.

    // Mapping from enemy key to its index in our vectors.
    std::unordered_map<size_t, size_t> enemyKeyIndexMap; // Key -> Index (For enemyID handling)

    // Vectors holding enemy data in a contiguous manner.
    std::vector<size_t> enemyKeyList; // Index -> key (reverse mapping)
    std::vector<Enemy::RenderInfo> enemyRenderInfos; // Index -> render info
    std::vector<InfoType> enemyUpdateFlags; // Index -> update flags

public:
    EnemyRenderer() : enemyVertices(sf::Quads) {}

    // Adds a new enemy and appends its quad to the vertex array.
    void addEnemy(const Enemy::RenderInfo& info, const size_t& key)
    {
        // New enemy index is the current count.
        const size_t index = enemyKeyList.size();
        // Adding all relevant elements
        enemyKeyList.push_back(key);
        enemyRenderInfos.push_back(info);
        enemyUpdateFlags.push_back(InfoType::EMPTY_INFO);

        // Store the mapping from key to index.
        enemyKeyIndexMap[key] = index;

        // Expand the vertex array by 4 vertices.
        const size_t prevVertexCount = enemyVertices.getVertexCount();
        enemyVertices.resize(prevVertexCount + 4);

        // Compute quad vertices.
        sf::Vertex* quad = &enemyVertices[prevVertexCount];
        const sf::Vector2f pos = info.pos;
        const sf::Vector2f halfSize = info.size / 2.0f;

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
        auto it = enemyKeyIndexMap.find(key);
        if (it == enemyKeyIndexMap.end())
            return; // Key not found

        size_t index = it->second;
        enemyRenderInfos[index].pos = position;
        enemyUpdateFlags[index] |= InfoType::POSITION;
    }

    void setSize(const size_t key, const sf::Vector2f& size)
    {
        auto it = enemyKeyIndexMap.find(key);
        if (it == enemyKeyIndexMap.end())
            return;

        size_t index = it->second;
        enemyRenderInfos[index].size = size;
        enemyUpdateFlags[index] |= InfoType::SIZE;
    }

    void setVelocity(const size_t key, const sf::Vector2f& velocity)
    {
        auto it = enemyKeyIndexMap.find(key);
        if (it == enemyKeyIndexMap.end())
            return;

        size_t index = it->second;
        enemyRenderInfos[index].velocity = velocity;
        // Velocity doesn't need an update flag since tick() directly applies it.
    }

    void setColor(const size_t key, const sf::Color& color)
    {
        auto it = enemyKeyIndexMap.find(key);
        if (it == enemyKeyIndexMap.end())
            return;

        size_t index = it->second;
        enemyRenderInfos[index].color = color;
        enemyUpdateFlags[index] |= InfoType::COLOR;
    }

    // Updates the vertex quad at the specified index if needed.
    void updateVertexQuad(size_t index)
    {
        const InfoType flags = enemyUpdateFlags[index];
        const size_t vertexStart = index * 4;

        // Update quad positions if POSITION or SIZE flag is set.
        if ((flags & InfoType::POSITION) != InfoType::EMPTY_INFO ||
            (flags & InfoType::SIZE) != InfoType::EMPTY_INFO)
        {
            const sf::Vector2f pos = enemyRenderInfos[index].pos;
            const sf::Vector2f halfSize = enemyRenderInfos[index].size / 2.f;
            enemyVertices[vertexStart + 0].position = pos + sf::Vector2f(-halfSize.x, -halfSize.y);
            enemyVertices[vertexStart + 1].position = pos + sf::Vector2f(halfSize.x, -halfSize.y);
            enemyVertices[vertexStart + 2].position = pos + sf::Vector2f(halfSize.x, halfSize.y);
            enemyVertices[vertexStart + 3].position = pos + sf::Vector2f(-halfSize.x, halfSize.y);
        }

        // Update color if COLOR flag is set.
        if ((flags & InfoType::COLOR) != InfoType::EMPTY_INFO)
        {
            const sf::Color color = enemyRenderInfos[index].color;
            for (int i = 0; i < 4; i++)
            {
                enemyVertices[vertexStart + i].color = color;
            }
        }
        // Reset update flag.
        enemyUpdateFlags[index] = InfoType::EMPTY_INFO;
    }

    // Removes an enemy given its key by swapping with the last enemy.
    void removeEnemy(const size_t& key)
    {
        auto it = enemyKeyIndexMap.find(key);
        if (it == enemyKeyIndexMap.end())
            return; // Enemy not found

        const size_t indexToRemove = it->second;
        const size_t lastIndex = enemyKeyList.size() - 1;

        // Erase the mapping for the enemy to be removed.
        enemyKeyIndexMap.erase(it);

        // If index to remove is not last, make it last by swapping
        if (indexToRemove != lastIndex)
        {
            // Get the key of the enemy that is currently last.
            const size_t lastKey = enemyKeyList[lastIndex];
            // Update the key-index mapping for the swapped enemy.
            enemyKeyIndexMap[lastKey] = indexToRemove;

            // Swap the enemy key.
            enemyKeyList[indexToRemove] = enemyKeyList[lastIndex];
            // Swap the render info.
            enemyRenderInfos[indexToRemove] = enemyRenderInfos[lastIndex];
            // Swap the update flags.
            enemyUpdateFlags[indexToRemove] = enemyUpdateFlags[lastIndex];

            // Swap the vertex quads in the vertex array.
            size_t vertexStartRemove = indexToRemove * 4;
            size_t vertexStartLast = lastIndex * 4;
            for (size_t i = 0; i < 4; i++)
            {
                std::swap(enemyVertices[vertexStartRemove + i], enemyVertices[vertexStartLast + i]);
            }
        }
        // Removing the last enemy, just pop back.
        enemyKeyList.pop_back();
        enemyRenderInfos.pop_back();
        enemyUpdateFlags.pop_back();
        // Remove the last quad from the vertex array.
        enemyVertices.resize(enemyVertices.getVertexCount() - 4);
    }

    // Tick: update enemy positions based on velocity and apply pending changes.
    void tick(const float& deltaTime)
    {
        // Update positions using velocity.
        for (size_t i = 0; i < enemyKeyList.size(); i++)
        {
            sf::Vector2f offset = enemyRenderInfos[i].velocity * deltaTime;
            if (offset != sf::Vector2f(0.f, 0.f))
            {
                enemyRenderInfos[i].pos += offset;
                size_t vertexStart = i * 4;
                for (size_t j = 0; j < 4; j++)
                {
                    enemyVertices[vertexStart + j].position += offset;
                }
            }
        }

        // Process any queued update flags.
        for (size_t i = 0; i < enemyKeyList.size(); i++)
        {
            // If the flag has any non-empty info, we process it.
            if (enemyUpdateFlags[i] & ~InfoType::EMPTY_INFO)
            {
                updateVertexQuad(i);
            }
        }
    }

    // Draws the complete vertex array at once.
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
    {
        target.draw(enemyVertices, states);
    }
};