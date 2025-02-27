#pragma once
#include <SFML/Graphics.hpp>

// Collision Interface, works with CollisionBox (CollisionComponent)
class ICollidable
{
private:
	// Collision tick prefab
	virtual void tick_collision(const float& deltaTime) {};
public:
	// Pure virtual function that must be implemented by any class using this interface.
	virtual ICollidable* getCollision() = 0;
	virtual sf::FloatRect getCollisionBounds()
	{
		return getCollision()->getCollisionBounds();
	}
	// Checking for Collision
	virtual bool isColliding(const sf::FloatRect& otherBound)
	{
		return getCollision()->isColliding(otherBound);
	}
	virtual bool isColliding(const sf::Vector2f& otherPos)
	{
		return getCollision()->isColliding(otherPos);
	}
	// Actual event for onCollison logic
	virtual void onCollision(ICollidable* other) {};

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

	void setPos(const sf::Vector2f& newPos)
	{
		pos = newPos;
		collisionRect.top = pos.y - (size.y / 2.0f);
		collisionRect.left = pos.x - (size.x / 2.0f);
	}
	void setSize(const sf::Vector2f& newSize)
	{
		size = newSize;
		collisionRect.top = pos.y - (size.y / 2.0f);
		collisionRect.left = pos.x - (size.x / 2.0f);
		collisionRect.width = size.x;
		collisionRect.height = size.y;
	}
	sf::Vector2f getSize() const { return size; }
	sf::Vector2f getPos() const { return pos; }
	// ICollidable
	ICollidable* getCollision() override { return nullptr; }
	sf::FloatRect getCollisionBounds() override { return collisionRect; }
	bool isColliding(const sf::FloatRect& otherBound) override
	{	
		return collisionRect.intersects(otherBound);
	}
	bool isColliding(const sf::Vector2f& otherPos) override
	{
		return collisionRect.contains(otherPos);
	}
};