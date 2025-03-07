// ProjectileRenderer.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "Functions.h"
#include "Projectile.h"

class ProjectileRenderer : public sf::Drawable {
private:
    // Vertex array that will store a quad (4 vertices) per projectile.
    sf::VertexArray projectileVertices; // Always in Quads mode.

    // Mapping from projectile key to its index.
    std::unordered_map<size_t, size_t> projectileKeyIndexMap;

    // Contiguous storage for keys and render info.
    std::vector<size_t> projectileKeyList; // Reverse mapping: index -> key.
    std::vector<IMovable::RenderInfo> projectileRenderInfos; // index -> render info.
    std::vector<InfoType> projectileUpdateFlags; // index -> update flags (using InfoType values).

public:
    ProjectileRenderer() : projectileVertices(sf::Quads)
        {}

    // Add a new projectile. Key can be a unique ID (or hash) for the projectile.
    void addProjectile(const IMovable::RenderInfo& info, const size_t& key)
    {
        std::cout << "Renderer added bullet" << std::endl;
        // New projectile index is the current count.
        const size_t index = projectileKeyList.size();
        // Adding all relevant elements
        projectileKeyList.push_back(key);
        projectileRenderInfos.push_back(info);
        projectileUpdateFlags.push_back(EMPTY_INFO);

        // Store the mapping.
        projectileKeyIndexMap[key] = index;

        // Expand the vertex array by 4 vertices (one quad per projectile).
        const size_t prevVertexCount = projectileVertices.getVertexCount();
        projectileVertices.resize(prevVertexCount + 4);

        // Compute the quad vertices.
        sf::Vertex* quad = &projectileVertices[prevVertexCount];
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

    // Setter functions to update individual RenderInfo fields.
    void setPosition(const size_t key, const sf::Vector2f& position)
    {
        auto it = projectileKeyIndexMap.find(key);
        if (it == projectileKeyIndexMap.end())
            return;

        size_t index = it->second;
        projectileRenderInfos[index].pos = position;
        projectileUpdateFlags[index] |= POSITION;
    }

    void setSize(const size_t key, const sf::Vector2f& size)
    {
        auto it = projectileKeyIndexMap.find(key);
        if (it == projectileKeyIndexMap.end())
            return;

        size_t index = it->second;
        projectileRenderInfos[index].size = size;
        projectileUpdateFlags[index] |= SIZE;
    }

    void setVelocity(const size_t key, const sf::Vector2f& velocity)
    {
        auto it = projectileKeyIndexMap.find(key);
        if (it == projectileKeyIndexMap.end())
            return;

        size_t index = it->second;
        projectileRenderInfos[index].velocity = velocity;
        // Velocity doesn't need an update flag since tick() directly applies it.
    }

    void setColor(const size_t key, const sf::Color& color)
    {
        auto it = projectileKeyIndexMap.find(key);
        if (it == projectileKeyIndexMap.end())
            return;

        size_t index = it->second;
        projectileRenderInfos[index].color = color;
        projectileUpdateFlags[index] |= COLOR;
    }

    void setRotation(const size_t key, float rotation)
    {
        auto it = projectileKeyIndexMap.find(key);
        if (it == projectileKeyIndexMap.end())
            return;

        size_t index = it->second;
        projectileRenderInfos[index].rot = rotation;
        projectileUpdateFlags[index] |= ROTATION;
    }

    // Update the quad for a given projectile index based on its update flags.
    void updateVertexQuad(size_t index)
    {
        const InfoType flags = projectileUpdateFlags[index];
        const size_t vertexStart = index * 4;

        // No update needed if there are no flags.
        if ((flags & (POSITION | SIZE | ROTATION)) == EMPTY_INFO &&
            (flags & COLOR) == EMPTY_INFO)
            return; // No update needed.

        // Update positions if POSITION, SIZE, or ROTATION flags are set.
        if (flags & (POSITION | SIZE | ROTATION))
        {
            const sf::Vector2f pos = projectileRenderInfos[index].pos;
            const sf::Vector2f halfSize = projectileRenderInfos[index].size / 2.f;

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
                float rotation = projectileRenderInfos[index].rot; // in degrees

                float rad = rotation * pi / 180.f;
                float cosA = std::cos(rad);
                float sinA = std::sin(rad);

                for (int i = 0; i < 4; i++)
                {
                    sf::Vector2f rotated;
                    rotated.x = corners[i].x * cosA - corners[i].y * sinA;
                    rotated.y = corners[i].x * sinA + corners[i].y * cosA;
                    projectileVertices[vertexStart + i].position = pos + rotated;
                }
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    // Without rotation, update quad positions directly.
                    projectileVertices[vertexStart + i].position = pos + corners[i];
                }
            }
        }

        // Update color if the COLOR flag is set.
        if (flags & COLOR)
        {
            const sf::Color color = projectileRenderInfos[index].color;
            for (int i = 0; i < 4; i++)
            {
                projectileVertices[vertexStart + i].color = color;
            }
        }
        // Reset update flag.
        projectileUpdateFlags[index] = EMPTY_INFO;
    }

    // Remove a projectile by key (using the swap-with-last method).
    void removeProjectile(const size_t& key)
    {
        auto it = projectileKeyIndexMap.find(key);
        if (it == projectileKeyIndexMap.end())
            return; // Key not found

        const size_t indexToRemove = it->second;
        const size_t lastIndex = projectileKeyList.size() - 1;

        // Erase mapping for the projectile to be removed.
        projectileKeyIndexMap.erase(it);

        // If not the last projectile, swap with last.
        if (indexToRemove != lastIndex) {
            // Get the key of the projectile that is currently last.
            const size_t lastKey = projectileKeyList[lastIndex];
            // Update the key-index mapping for the swapped projectile.
            projectileKeyIndexMap[lastKey] = indexToRemove;

            // Swap the projectile key.
            projectileKeyList[indexToRemove] = projectileKeyList[lastIndex];
            // Swap the render info.
            projectileRenderInfos[indexToRemove] = projectileRenderInfos[lastIndex];
            // Swap the update flags.
            projectileUpdateFlags[indexToRemove] = projectileUpdateFlags[lastIndex];

            // Swap the vertex quads in the vertex array.
            size_t vertexStartRemove = indexToRemove * 4;
            size_t vertexStartLast = lastIndex * 4;
            for (size_t i = 0; i < 4; i++)
            {
                std::swap(projectileVertices[vertexStartRemove + i], projectileVertices[vertexStartLast + i]);
            }
        }
        // Removing the last projectile, just pop_back.
        projectileKeyList.pop_back();
        projectileRenderInfos.pop_back();
        projectileUpdateFlags.pop_back();
        // Remove the last quad from the vertex array.
        projectileVertices.resize(projectileVertices.getVertexCount() - 4);
    }

    // Tick: update projectile positions based on velocity and process any pending update flags.
    void tick(const float& deltaTime)
    {
        // Update positions using the velocity stored in RenderInfo.
        for (size_t i = 0; i < projectileKeyList.size(); i++)
        {
            sf::Vector2f offset = projectileRenderInfos[i].velocity * deltaTime;
            if (offset != sf::Vector2f(0.f, 0.f))
            {
                projectileRenderInfos[i].pos += offset;
                const size_t vertexStart = i * 4;
                for (size_t j = 0; j < 4; j++)
                {
                    projectileVertices[vertexStart + j].position += offset;
                }
            }
        }

        // Process any queued update flags.
        for (size_t i = 0; i < projectileKeyList.size(); i++)
        {
            // If the flag has any non-empty info, we process it.
            if (projectileUpdateFlags[i] != EMPTY_INFO)
                updateVertexQuad(i);
        }
    }

    // Draw the complete vertex array.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
    {
        target.draw(projectileVertices, states);
    }
};
