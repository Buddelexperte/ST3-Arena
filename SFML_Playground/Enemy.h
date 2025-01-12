#pragma once
#include "GameInstance.h"

class Enemy : public sf::Drawable
{
private:
	sf::RectangleShape shape;
	sf::Vector2f velocity;
public:
	Enemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
		: velocity(0.0f, 0.0f)
	{
		setPosition(pos);
		setSize(size);
		setColor(color);
	}

	void setPosition(const sf::Vector2f& pos)
	{
		shape.setPosition(pos);
	}

	void setSize(const sf::Vector2f& size)
	{
		shape.setSize(size);
		shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
	}

	void setColor(const sf::Color& color)
	{
		shape.setFillColor(color);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(shape, states);
	}
};

class EnemyPool
{
private:
	std::queue<std::unique_ptr<Enemy>> pool;

public:
	std::unique_ptr<Enemy> get(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
	{
		if (pool.empty())
			return std::make_unique<Enemy>(pos, size, color);
		std::unique_ptr<Enemy> enemy = std::move(pool.front());
		pool.pop();
		enemy->setPosition(pos);
		enemy->setSize(size);
		enemy->setColor(color);
		return enemy;
	}

	void release(std::unique_ptr<Enemy> enemyPtr)
	{
		pool.push(std::move(enemyPtr));
	}
};

class EnemyRenderer : public sf::Drawable
{
private:
    sf::VertexArray enemies; // Store enemies as quads
    std::vector<sf::Vector2f> velocities; // All enemy velocities
    size_t numEnemies; // Track the number of active enemies

public:
    EnemyRenderer()
        : enemies(sf::Quads), numEnemies(0)
    {}

    void addEnemy(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color)
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
            velocities.push_back(sf::Vector2f(0.f, 0.f));
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

        // Set velocity for the enemy
        velocities[index] = sf::Vector2f(0.f, 0.f);

        // Increment the number of active enemies
        numEnemies++;
    }

    void setVelocity(size_t index, const sf::Vector2f& velocity)
    {
        if (index < velocities.size()) velocities[index] = velocity;
    }

    void removeEnemy(size_t index)
    {
        if (index < numEnemies) {
            // Swap with the last active enemy
            size_t lastIndex = numEnemies - 1;

            // Swap vertices
            for (size_t i = 0; i < 4; ++i) {
                std::swap(enemies[index * 4 + i], enemies[lastIndex * 4 + i]);
            }

            // Swap velocities
            std::swap(velocities[index], velocities[lastIndex]);

            // Decrease the number of active enemies
            numEnemies--;
        }
    }

    void tick(float deltaTime) {
        for (size_t i = 0; i < numEnemies; ++i)
        {
            sf::Vector2f offset = velocities[i] * deltaTime;

            // Update all 4 vertices of the quad
            for (size_t j = 0; j < 4; ++j)
            {
                enemies[i * 4 + j].position += offset;
            }
        }
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
    {
        for (size_t i = 0; i < numEnemies; ++i)
        {
            target.draw(&enemies[i * 4], 4, sf::Quads, states);
        }
    }
};
