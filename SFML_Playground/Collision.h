#pragma once
#include <SFML/Graphics.hpp>

// Collision Interface, works with CollisionBox (CollisionComponent)
class ICollidable
{
private:
	// Collision tick prefab
	virtual void tick_collision(const float& deltaTime) = 0;
public:
	struct RenderInfo {
		sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f size = sf::Vector2f(100.0f, 100.0f);
		sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
		sf::Color color = sf::Color::White;
	};
	// Pure virtual function that must be implemented by any class using this interface.
	virtual sf::FloatRect getCollisionBounds() const = 0;
	// Checking for Collision
	virtual bool isColliding(const sf::FloatRect& otherBound) const = 0;
	virtual bool isColliding(const sf::Vector2f& otherPos) const = 0;
	// Actual event for onCollison logic
	virtual void onCollision(ICollidable* other) = 0;

	// Virtual destructor is important for proper cleanup.
	virtual ~ICollidable() = default;
};

// Collision Component, works with ICollision (Collision Interface)
class CollisionBox : public ICollidable
{
protected:
	sf::FloatRect collisionRect;
	sf::Vector2f pos;
	sf::Vector2f size;
public:
	CollisionBox(const sf::Vector2f& pos, const sf::Vector2f& size)
		: pos(pos), size(size)
	{
		collisionRect.top = pos.y - (size.y / 2.0f);
		collisionRect.left = pos.x - (size.x / 2.0f);
		collisionRect.width = size.x;
		collisionRect.height = size.y;
	}
	void setSize(const sf::Vector2f& newSize)
	{
		size = newSize;
		collisionRect.top = pos.y - (size.y / 2.0f);
		collisionRect.left = pos.x - (size.x / 2.0f);
		collisionRect.width = size.x;
		collisionRect.height = size.y;
	}
	void setPos(const sf::Vector2f& newPos)
	{
		pos = newPos;
		collisionRect.top = pos.y - (size.y / 2.0f);
		collisionRect.left = pos.x - (size.x / 2.0f);
	}
	sf::Vector2f getSize() const { return size; }
	sf::Vector2f getPos() const { return pos; }
	// ICollidable
	sf::FloatRect getCollisionBounds() const override { return collisionRect; }
	void tick_collision(const float& deltaTime) override {};
	bool isColliding(const sf::FloatRect& otherBound) const override
	{	
		return getCollisionBounds().intersects(otherBound);
	}
	bool isColliding(const sf::Vector2f& otherPos) const override
	{
		return getCollisionBounds().contains(otherPos);
	}

	void onCollision(ICollidable* other) {}
};

// TODO: Add Implementation