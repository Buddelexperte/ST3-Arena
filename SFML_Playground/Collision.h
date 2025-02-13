#pragma once
#include <SFML/Graphics.hpp>

// Collision Interface, works with CollisionBox (CollisionComponent)
class ICollidable
{
public:
	// Pure virtual function that must be implemented by any class using this interface.
	virtual sf::FloatRect getCollisionBounds() const = 0;
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
	sf::FloatRect getCollisionBounds() const override
	{
		return collisionRect;
	}
	// Check for collision with another FloatRect (collisionRectangle)
	bool isColliding(const sf::FloatRect& otherBound) const
	{
		return collisionRect.intersects(otherBound);
	}
	// Check for collision with a point (2-Float Vector)
	bool isColliding(const sf::Vector2f& otherPos) const
	{
		return collisionRect.contains(otherPos);
	}

	void onCollision(ICollidable* other) {}
};

// TODO: Add Implementation