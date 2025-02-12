#pragma once
#include <SFML/Graphics.hpp>

class C_Collision
{
protected:
	sf::FloatRect collisionRect;
	sf::Vector2f pos;
	sf::Vector2f size;
public:
	C_Collision(const sf::Vector2f& pos, const sf::Vector2f& size)
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
	sf::FloatRect getBounds() const
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
};
