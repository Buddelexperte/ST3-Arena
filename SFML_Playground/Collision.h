#pragma once
#include <SFML/Graphics.hpp>


class Collidable;
class Enemy;
class Player;
class Projectile;

// Collision interface Class for actors
class IHasCollision
{
public:
	virtual Collidable* getCollision() = 0;

	// Actual event for onCollison logic
	virtual void onCollision(IHasCollision* other)
		{}

	// Collision responses when the other object is of a specific type.
	virtual void collideWithEnemy(Enemy& enemy) {};
	virtual void collideWithPlayer(Player& player) {};
	virtual void collideWithProjectile(Projectile& projectile) {};
};

// Collision base class
class Collidable
{
private:
	IHasCollision* owner = nullptr;
	size_t collisionID = -1;
public:
	Collidable(IHasCollision* owner)
		: owner(owner)
	{ }
	// Collision ID managing
	virtual void setCollisionID(const size_t& newID)
		{ collisionID = newID; }

	virtual size_t getCollisionID() const
		{ return collisionID; }

	virtual sf::FloatRect getCollisionBounds() = 0; // Abstract

	// Checking for Collision
	virtual bool isColliding(Collidable* other)
		{ return isColliding(other->getCollisionBounds()); }
	
	virtual bool isColliding(const sf::FloatRect& otherBound)
	{
		return getCollisionBounds().intersects(otherBound);
	}
	virtual bool isColliding(const sf::Vector2f& otherPos)
	{
		return getCollisionBounds().contains(otherPos);
	}

	void onCollision(Collidable* other)
	{
		owner->onCollision(other->owner);
	}

	// Virtual destructor is important for proper cleanup.
	virtual ~Collidable() = default;
};

// Collision Component, works with ICollision (Collision Interface)
class CollisionBox : public Collidable
{
protected:
	sf::FloatRect collisionRect;
	sf::Vector2f pos;
	sf::Vector2f size;
public:
	CollisionBox(IHasCollision* owner, const sf::Vector2f& pos, const sf::Vector2f& size)
		: pos(pos), size(size), Collidable(owner)
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
	sf::Vector2f getSize() const 
		{ return size; }
	sf::Vector2f getPos() const 
		{ return pos; }
	// ICollidable
	sf::FloatRect getCollisionBounds() override 
		{ return collisionRect; }
};