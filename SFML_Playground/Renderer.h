#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "BaseTypes.h"
#include "Functions.h"
#include "RenderInfo.h"

class EntityRenderer : public sf::Drawable
{
private:
    sf::VertexArray entityVertices;                             // Stores all entity quads.
    std::unordered_map<size_t, size_t> entityKeyIndexMap;       // Maps keys to indices.

    std::vector<size_t> entityKeyList;                          // Reverse mapping: index -> key.
    std::vector<IMovable::RenderInfo> entityRenderInfos;        // Index -> render info.
    std::vector<InfoType> entityUpdateFlags;                    // Index -> update flags.

public:
    EntityRenderer() : entityVertices(sf::Quads) {}

    // Add an entity (Projectile, Enemy, etc.).
    void addEntity(const IMovable::RenderInfo& info, const size_t& key)
    {
        // New projectile index is the current count.
        const size_t index = entityKeyList.size();
        // Adding all relevant elements
        entityKeyList.push_back(key);
        entityRenderInfos.push_back(info);
        entityUpdateFlags.push_back(EMPTY_INFO);

        // Store the mapping.
        entityKeyIndexMap[key] = index;

        // Expand the vertex array by 4 vertices (one quad per projectile).
        const size_t prevVertexCount = entityVertices.getVertexCount();
        entityVertices.resize(prevVertexCount + 4);

        // Compute the quad vertices.
        sf::Vertex* quad = &entityVertices[prevVertexCount];
        const sf::Vector2f pos = info.pos;
        const sf::Vector2f halfSize = info.size / 2.f;

        quad[0].position = pos + sf::Vector2f(-halfSize.x, -halfSize.y);
        quad[1].position = pos + sf::Vector2f(halfSize.x, -halfSize.y);
        quad[2].position = pos + sf::Vector2f(halfSize.x, halfSize.y);
        quad[3].position = pos + sf::Vector2f(-halfSize.x, halfSize.y);

        // Set color for all vertices.
        for (int i = 0; i < 4; i++)
        {
            quad[i].color = info.color;
        }
    }

    // Update functions for different properties.
    void setPosition(const size_t key, const sf::Vector2f& position)
    {
        auto it = entityKeyIndexMap.find(key);
        if (it == entityKeyIndexMap.end())
            return;

        size_t index = it->second;
        entityRenderInfos[index].pos = position;
        entityUpdateFlags[index] |= POSITION;
    }

    void setSize(const size_t key, const sf::Vector2f& size)
    {
        auto it = entityKeyIndexMap.find(key);
        if (it == entityKeyIndexMap.end())
            return;

        size_t index = it->second;
        entityRenderInfos[index].size = size;
        entityUpdateFlags[index] |= SIZE;
    }

    void setVelocity(const size_t key, const sf::Vector2f& velocity)
    {
        auto it = entityKeyIndexMap.find(key);
        if (it == entityKeyIndexMap.end())
            return;

        size_t index = it->second;
        entityRenderInfos[index].velocity = velocity;
        // Velocity doesn't need an update flag since tick() directly applies it.
    }

    void setColor(const size_t key, const sf::Color& color)
    {
        auto it = entityKeyIndexMap.find(key);
        if (it == entityKeyIndexMap.end())
            return;

        size_t index = it->second;
        entityRenderInfos[index].color = color;
        entityUpdateFlags[index] |= COLOR;
    }

    void setRotation(const size_t key, float rotation)
    {
        auto it = entityKeyIndexMap.find(key);
        if (it == entityKeyIndexMap.end())
            return;

        size_t index = it->second;
        entityRenderInfos[index].rot = rotation;
        entityUpdateFlags[index] |= ROTATION;
    }

    // Updates a quad for a given entity index based on flags.
    void updateVertexQuad(size_t index, bool fullUpdate = false)
    {
        const size_t vertexStart = index * 4;
        const InfoType flags = entityUpdateFlags[index];
        auto& info = entityRenderInfos[index];

        // No update needed if there are no flags.
        if (flags == EMPTY_INFO)
            return;

        // Update positions if POSITION, SIZE, or ROTATION flags are set.
        if (fullUpdate || flags & (POSITION | SIZE | ROTATION))
        {
            const sf::Vector2f pos = info.pos;
            const sf::Vector2f halfSize = info.size / 2.0f;

            // Define local (unrotated) corners.
            sf::Vector2f corners[4] = {
                sf::Vector2f(-halfSize.x, -halfSize.y),
                sf::Vector2f(halfSize.x, -halfSize.y),
                sf::Vector2f(halfSize.x,  halfSize.y),
                sf::Vector2f(-halfSize.x,  halfSize.y)
            };

            // If ROTATION flag is set, rotate the corners.
            if (flags & ROTATION)
            {
                float rotation = info.rot; // in degrees

                double rad = rotation * pi / 180.f;
                float cosA = static_cast<float>(std::cos(rad));
                float sinA = static_cast<float>(std::sin(rad));

                for (auto& corner : corners)
                {
                    float x = corner.x * cosA - corner.y * sinA;
                    float y = corner.x * sinA + corner.y * cosA;
                    corner = { x, y };
                }
            }

            for (int i = 0; i < 4; i++)
            {
                // With and Without rotation, update quad positions directly.
                entityVertices[vertexStart + i].position = pos + corners[i];
            }
        }

        // Update color if the COLOR flag is set.
        if (flags & COLOR)
        {
            const sf::Color color = info.color;
            for (int i = 0; i < 4; i++)
            {
                entityVertices[vertexStart + i].color = color;
            }
        }

        // Reset update flag.
        entityUpdateFlags[index] = EMPTY_INFO;
    }

    // Removes an entity using the swap-with-last method.
    void removeEntity(const size_t key)
    {
        auto it = entityKeyIndexMap.find(key);
        if (it == entityKeyIndexMap.end())
            return;

        size_t indexToRemove = entityKeyIndexMap[key];
        size_t lastIndex = entityKeyList.size() - 1;

        entityKeyIndexMap.erase(it);
        if (indexToRemove != lastIndex)
        {
            size_t lastKey = entityKeyList[lastIndex];
            entityKeyIndexMap[lastKey] = indexToRemove;

            entityKeyList[indexToRemove] = lastKey;
            entityRenderInfos[indexToRemove] = entityRenderInfos[lastIndex];
            entityUpdateFlags[indexToRemove] = entityUpdateFlags[lastIndex];

            size_t vertexStartRemove = indexToRemove * 4;
            size_t vertexStartLast = lastIndex * 4;
            for (size_t i = 0; i < 4; i++)
                std::swap(entityVertices[vertexStartRemove + i], entityVertices[vertexStartLast + i]);
        }

        entityKeyList.pop_back();
        entityRenderInfos.pop_back();
        entityUpdateFlags.pop_back();
        entityVertices.resize(entityVertices.getVertexCount() - 4);
    }

    // Tick function to update positions based on velocity.
    void tick(float deltaTime)
    {
        for (size_t i = 0; i < entityKeyList.size(); i++)
        {
            sf::Vector2f offset = entityRenderInfos[i].velocity * deltaTime;
            if (offset != sf::Vector2f(0.f, 0.f))
            {
                entityRenderInfos[i].pos += offset;
                size_t vertexStart = i * 4;
                for (size_t j = 0; j < 4; j++)
                    entityVertices[vertexStart + j].position += offset;
            }
        }

        for (size_t i = 0; i < entityKeyList.size(); i++)
            if (entityUpdateFlags[i] != EMPTY_INFO)
                updateVertexQuad(i);
    }

    // Draw function for SFML rendering.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(entityVertices, states);
    }
};
